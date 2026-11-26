#!/usr/bin/env python3
"""List the function definitions in a C file, with exact source spans.

tree-sitter-c rather than pycparser, for three reasons that all bite this
tree:

  * pycparser rejects every GNU extension. `__asm__` is a syntax error to it
    in all three positions we use it: the bare statement INCLUDE_ASM expands
    to, the asm label on an aliased extern (`extern u32 D_80084904a[]
    __asm__("D_80084904")`), and the `__asm__ volatile` in gte_inline.h. So
    is `__attribute__((section(".data")))`. Stripping them with -D is only
    half possible -- a function-like `-D'__asm__(...)='` never fires on
    `__asm__ volatile`, because cpp only expands a function-like macro when
    the very next token is `(` and never reconsiders it once the qualifier
    has been removed.

  * pycparser needs the source preprocessed first, and then its coords are
    only as good as the linemarkers. tools/m2ctx.py passes -E -P, which
    strips them, so every coord lands at an offset into the flattened
    stream. Dropping -P fixes the line numbers but not the next point.

  * pycparser records no end position for any node and no byte offsets at
    all, so line_end/byte_start/byte_end are not derivable from its AST.
    tree-sitter carries start_byte/end_byte/start_point/end_point on every
    node.

Not preprocessing is the other half of the win: spans refer to bytes in the
file on disk, so a caller can slice the original text back out, and an
INCLUDE_ASM line stays an unexpanded macro invocation instead of becoming a
function named __maspsx_include_asm_hack_*.

Two costs. There is no preprocessor, so both arms of an #if are parsed and a
function defined in a dead arm is still reported; nothing under csrc/ does
that today. And tree-sitter-c's grammar has gaps on this dialect -- see
_recover_kr below, and note that an `__attribute__` sitting between a
declarator and its initialiser (`s32 D_8040923C __attribute__((section(
".data"))) = 0;`) produces an ERROR node. That one is harmless here: it is
always file-scope data, so no function span depends on it. A stray label
before a closing brace (`ok: }`) and a `register s32 zz __asm__("$6") = b`
local also error, and in both cases the enclosing function still parses with
the correct span.
"""

import contextlib
import dataclasses
import fcntl
import os
import pathlib
from typing import Generator, IO, Iterator

import tree_sitter as ts
import tree_sitter_c

Type = str

# The project's C style: 4 spaces, Allman braces.
_INDENT = "    "


class EditError(Exception):
    """A function could not be edited."""


_LANGUAGE = ts.Language(tree_sitter_c.language())
_PARSER = ts.Parser(_LANGUAGE)

# These wrap the function_declarator in `u8 *f()` and `void (*f)()`;
# array_declarator shows up on parameters.
_DECLARATOR_WRAPPERS = (
    "pointer_declarator",
    "parenthesized_declarator",
    "array_declarator",
)


@dataclasses.dataclass
class Function:
    name: str
    ret_type: Type
    params: list[tuple[Type, str]]
    # Empty unless the caller asked for it: bodies are ~72% of the bytes in a
    # whole-file listing (798KB vs 219KB for ovl_town.c's 1203 functions).
    body: str
    # Empty unless asked for, like `body`. The span fields describe the
    # definition alone, so a comment above it is outside byte_start.
    comment: str
    line_start: int
    line_end: int
    byte_start: int
    byte_end: int


def _adjacent(src: bytes, end: int, start: int) -> bool:
    """Is the gap between two nodes blank-line free?

    One newline means the next line; two means a blank line, which separates
    a comment from what follows rather than attaching it.
    """
    return src[end:start].count(b"\n") <= 1


def _comment_span(src: bytes, node: ts.Node) -> tuple[int, int] | None:
    """The comment block sitting immediately above `node`, if there is one.

    Walks back over contiguous comment siblings, so a run of several `//`
    lines reads as one block; this tree writes `/* */` headers, which are a
    single node already. A blank line anywhere in the run ends it, which is
    what keeps an unrelated comment higher up the file from being claimed.
    """
    first = None
    current = node
    while True:
        previous = current.prev_named_sibling
        if (previous is None or previous.type != "comment"
                or not _adjacent(src, previous.end_byte, current.start_byte)):
            break
        first = previous
        current = previous

    if first is None:
        return None
    return first.start_byte, node.prev_named_sibling.end_byte


def _text(src: bytes, node: ts.Node) -> str:
    return src[node.start_byte : node.end_byte].decode("utf-8", "replace")


def _squeeze(s: str) -> str:
    return " ".join(s.split())


def _name_node(node: ts.Node | None) -> ts.Node | None:
    """The first identifier in a declarator subtree -- the declared name."""
    if node is None:
        return None
    if node.type == "identifier":
        return node
    for child in node.named_children:
        found = _name_node(child)
        if found is not None:
            return found
    return None


def _abstract(src: bytes, declarator: ts.Node, name: ts.Node) -> str:
    """A declarator's text with the declared name cut out of it.

    Cutting rather than rebuilding gives a correct abstract declarator for
    every shape for free: `*n` -> `*`, `arr[4]` -> `[4]`,
    `(*cb)(int)` -> `(*)(int)`.
    """
    whole = _text(src, declarator)
    start = name.start_byte - declarator.start_byte
    end = name.end_byte - declarator.start_byte
    return whole[:start] + whole[end:]


def _declared(src: bytes, node: ts.Node) -> list[tuple[Type, str]]:
    """Every (type, name) a declaration or parameter_declaration declares.

    A list because a K&R parameter declaration may name several at once
    (`s32 a, b;`).
    """
    type_node = node.child_by_field_name("type")
    base = _text(src, type_node) if type_node is not None else ""

    out: list[tuple[Type, str]] = []
    for declarator in node.children_by_field_name("declarator"):
        name = _name_node(declarator)
        if name is None:
            continue
        out.append(
            (_squeeze(f"{base} {_abstract(src, declarator, name)}"), _text(src, name))
        )

    if not out:
        # An unnamed parameter: `void f(int)`, or a bare `void`.
        out.append((_squeeze(base), ""))
    return out


def _kr_types(src: bytes, declarations: list[ts.Node]) -> dict[str, Type]:
    """Map name -> type from the declaration list of a K&R definition."""
    types: dict[str, Type] = {}
    for declaration in declarations:
        for type_, name in _declared(src, declaration):
            if name:
                types[name] = type_
    return types


def _bare_type(src: bytes, node: ts.Node) -> str | None:
    """The text of a parameter_declaration that is a lone type name."""
    if node.child_by_field_name("declarator") is not None:
        return None
    children = node.named_children
    if len(children) != 1 or children[0].type != "type_identifier":
        return None
    return _text(src, children[0])


def _params(
    src: bytes, declarator: ts.Node, kr_types: dict[str, Type]
) -> list[tuple[Type, str]]:
    plist = declarator.child_by_field_name("parameters")
    if plist is None:
        return []

    out: list[tuple[Type, str]] = []
    for child in plist.named_children:
        if child.type == "variadic_parameter":
            out.append(("...", ""))
        elif child.type == "parameter_declaration":
            # With a pointer return type the grammar reads a K&R identifier
            # list as a list of type names, so `t` arrives as a lone
            # type_identifier. If the declaration list names it, it is a
            # parameter name. Otherwise it really is an unnamed parameter of
            # that type, which is the reading `void f(size_t)` wants.
            bare = _bare_type(src, child)
            if bare is not None and bare in kr_types:
                out.append((kr_types[bare], bare))
            else:
                out.extend(_declared(src, child))
        elif child.type == "identifier":
            # A K&R identifier list. The type lives in the declaration list;
            # absent from it, the parameter is an implicit int.
            name = _text(src, child)
            out.append((kr_types.get(name, "int"), name))

    # `(void)` is an empty parameter list, not a list holding a void.
    if out == [("void", "")]:
        return []
    return out


def _unwrap(node: ts.Node | None) -> tuple[ts.Node | None, int]:
    """Descend a declarator chain to the function_declarator, counting stars."""
    stars = 0
    while node is not None and node.type in _DECLARATOR_WRAPPERS:
        if node.type == "pointer_declarator":
            stars += 1
        node = node.child_by_field_name("declarator")
    return node, stars


def _build(
    src: bytes,
    type_node: ts.Node | None,
    declarator: ts.Node,
    stars: int,
    kr_types: dict[str, Type],
    body: ts.Node,
    start_byte: int,
    start_row: int,
    include_body: bool,
    comment: str,
) -> Function | None:
    name = _name_node(declarator.child_by_field_name("declarator"))
    if name is None:
        return None

    # The `type` field, plus the stars the declarator chain carried. Taking
    # the text ahead of the name instead would fold in a storage class.
    ret_type = _squeeze(_text(src, type_node)) if type_node is not None else ""
    if stars:
        ret_type = f"{ret_type} {'*' * stars}"

    return Function(
        name=_text(src, name),
        ret_type=ret_type,
        params=_params(src, declarator, kr_types),
        body=_text(src, body) if include_body else "",
        comment=comment,
        line_start=start_row + 1,
        line_end=body.end_point[0] + 1,
        byte_start=start_byte,
        byte_end=body.end_byte,
    )


def _function(src: bytes, node: ts.Node, include_body: bool = False,
              include_comment: bool = False) -> Function | None:
    """A Function from a function_definition node."""
    declarator, stars = _unwrap(node.child_by_field_name("declarator"))
    if declarator is None or declarator.type != "function_declarator":
        return None

    body = node.child_by_field_name("body")
    if body is None:
        return None

    # A K&R definition keeps its parameter types in declarations sitting
    # between the declarator and the body.
    declarations = [c for c in node.named_children if c.type == "declaration"]

    return _build(
        src,
        node.child_by_field_name("type"),
        declarator,
        stars,
        _kr_types(src, declarations),
        body,
        node.start_byte,
        node.start_point[0],
        include_body,
        _comment_text(src, node) if include_comment else "",
    )


def _comment_text(src: bytes, node: ts.Node) -> str:
    span = _comment_span(src, node)
    return "" if span is None else src[span[0]:span[1]].decode("utf-8", "replace")


def _absorbed_kr(src: bytes, declarator: ts.Node) -> dict[str, Type]:
    """The K&R declaration the grammar swallowed into the declarator.

    On the pointer-return misparse the first declaration-list line is pulled
    inside the function_declarator, trailing the parameter_list as a loose
    identifier/ERROR/identifier run. Re-parsing that text as its own
    declaration recovers it rather than reconstructing it by hand.
    """
    plist = declarator.child_by_field_name("parameters")
    if plist is None:
        return {}

    tail = src[plist.end_byte : declarator.end_byte].strip()
    if not tail:
        return {}

    fragment = tail + b";"
    declarations = [
        c for c in parse(fragment).root_node.named_children if c.type == "declaration"
    ]
    return _kr_types(fragment, declarations)


def _recover_kr(src: bytes, parent: ts.Node, include_body: bool = False,
                include_comment: bool = False) -> Iterator[Function]:
    """Recover a K&R definition whose return type is a pointer.

    tree-sitter-c parses `s32 f(a, b)` + declarations + body as a proper
    function_definition, but `u8 *f(t, b)` defeats it: the grammar commits to
    a declaration, strands an ERROR in the parameter_list, and leaves the
    body as a free-standing compound_statement sibling. So no
    function_definition is produced and the function would simply go missing
    -- csrc/ovl_town.c's func_8009368C is the one instance in this tree.

    The shape is specific enough to match on directly: a declaration whose
    declarator unwraps to a function_declarator, followed by zero or more
    declarations, followed by a compound_statement. Valid C never puts a
    compound_statement after a declaration at file scope.
    """
    children = parent.named_children
    for i, node in enumerate(children):
        if node.type != "declaration":
            continue
        declarator, stars = _unwrap(node.child_by_field_name("declarator"))
        if declarator is None or declarator.type != "function_declarator":
            continue

        j = i + 1
        while j < len(children) and children[j].type == "declaration":
            j += 1
        if j >= len(children) or children[j].type != "compound_statement":
            continue

        kr_types = _absorbed_kr(src, declarator)
        kr_types.update(_kr_types(src, list(children[i + 1 : j])))

        found = _build(
            src,
            node.child_by_field_name("type"),
            declarator,
            stars,
            kr_types,
            children[j],
            node.start_byte,
            node.start_point[0],
            include_body,
            _comment_text(src, node) if include_comment else "",
        )
        if found is not None:
            yield found


def parse(src: bytes) -> ts.Tree:
    """Parse C source. Check `tree.root_node.has_error` if that matters.

    tree-sitter recovers rather than raising, so a malformed file yields a
    tree with ERROR nodes in it and the functions around them still read
    correctly.
    """
    return _PARSER.parse(src)


def iter_functions(src: bytes, tree: ts.Tree | None = None,
                   include_body: bool = False,
                   include_comment: bool = False) -> Iterator[Function]:
    """Yield every function definition in `src`, in source order.

    Walks the whole tree rather than the top-level children, so definitions
    inside a preproc_if arm or a linkage_specification are found too. Does
    not descend into a function once matched, so a GCC nested function is
    reported only as part of its parent's body.
    """
    if tree is None:
        tree = parse(src)

    found: dict[int, Function] = {}
    stack = [tree.root_node]
    while stack:
        node = stack.pop()
        if node.type == "function_definition":
            function = _function(src, node, include_body, include_comment)
            if function is not None:
                found[function.byte_start] = function
                continue
        if node.has_error:
            for function in _recover_kr(src, node, include_body,
                                        include_comment):
                found.setdefault(function.byte_start, function)
        stack.extend(node.named_children)

    for _, function in sorted(found.items()):
        yield function


def describe_function(filepath: str | pathlib.Path, name: str,
                      include_body: bool = False,
                      include_comment: bool = False) -> Function | None:
    """The one function defined as `name`, or None. Never a declaration.

    `include_body` fills in Function.body; it is left empty otherwise, since
    a caller holding a span can slice the text itself. `include_comment`
    fills in the comment block immediately above the definition -- in this
    tree, the header comment recording how the body was derived.
    """
    src = pathlib.Path(filepath).read_bytes()
    for function in iter_functions(src, include_body=include_body,
                                   include_comment=include_comment):
        if function.name == name:
            return function
    return None


def describe_functions(filepath: str | pathlib.Path,
                       include_body: bool = False,
                       include_comment: bool = False) -> list[Function]:
    """The function definitions in a file, in source order.

    No preprocessor runs, so there is nothing to configure: spans index the
    bytes on disk. `include_body` fills in Function.body, which is left
    empty otherwise because bodies dominate the size of a whole-file list;
    `include_comment` fills in the comment block above each definition.
    """
    src = pathlib.Path(filepath).read_bytes()
    return list(iter_functions(src, include_body=include_body,
                               include_comment=include_comment))


@dataclasses.dataclass
class Stub:
    """An INCLUDE_ASM site: a function this unit has not landed yet."""
    name: str
    folder: str
    line: int
    byte_start: int
    byte_end: int


def describe_stubs(filepath: str | pathlib.Path) -> list[Stub]:
    """The INCLUDE_ASM stubs in a file, in source order.

    Half of what is in a unit here is stubs, and no C parser sees one as a
    function, so a listing alone answers "what is in this file" only for the
    landed half.

    Found structurally rather than by matching text. A comment that mentions
    INCLUDE_ASM is a `comment` node and a mention inside a string literal is
    inside a `string_literal`, so neither can be mistaken for a call -- which
    is how `grep -c INCLUDE_ASM` once reported one stub for a unit with none.
    The macro takes TWO arguments, a folder and a name; a pattern written for
    three once reported "3 written" per unit when the truth was 8, 7, 6 and
    5. Here the shape is read off the argument_list instead of assumed, and a
    call that is not (string, identifier) is not a stub.

    INCLUDE_RODATA is deliberately not included: a stub stands for a function.
    """
    src = pathlib.Path(filepath).read_bytes()

    found: dict[int, Stub] = {}
    stack = [parse(src).root_node]
    while stack:
        node = stack.pop()
        if node.type == "call_expression":
            target = node.child_by_field_name("function")
            arguments = node.child_by_field_name("arguments")
            if (target is not None and arguments is not None
                    and target.type == "identifier"
                    and _text(src, target) == "INCLUDE_ASM"):
                args = arguments.named_children
                if (len(args) == 2 and args[0].type == "string_literal"
                        and args[1].type == "identifier"):
                    found[node.start_byte] = Stub(
                        name=_text(src, args[1]),
                        folder=_text(src, args[0]).strip('"'),
                        line=node.start_point[0] + 1,
                        byte_start=node.start_byte,
                        byte_end=node.end_byte,
                    )
                    continue
        stack.extend(node.named_children)

    return [stub for _, stub in sorted(found.items())]


def _find_definition(
    src: bytes, name: str
) -> tuple[ts.Node, ts.Node, ts.Node, ts.Node]:
    """The (definition, declarator, parameter_list, body) of a named function.

    Only a real function_definition qualifies. A K&R definition with a
    pointer return type reaches list_functions through _recover_kr, which
    reconstructs a span out of a misparse; there is no coherent node to
    rewrite, so editing one is refused rather than attempted.
    """
    matches: list[tuple[ts.Node, ts.Node, ts.Node, ts.Node]] = []

    stack = [parse(src).root_node]
    while stack:
        node = stack.pop()
        if node.type == "function_definition":
            declarator, _ = _unwrap(node.child_by_field_name("declarator"))
            body = node.child_by_field_name("body")
            if declarator is not None and body is not None:
                found = _name_node(declarator.child_by_field_name("declarator"))
                plist = declarator.child_by_field_name("parameters")
                if (
                    found is not None
                    and plist is not None
                    and _text(src, found) == name
                ):
                    matches.append((node, declarator, plist, body))
                    continue
        stack.extend(node.named_children)

    if not matches:
        recovered = any(f.name == name for f in iter_functions(src))
        if recovered:
            raise EditError(
                f"{name} only parses through the K&R pointer-return recovery, "
                "which has no node to rewrite; edit it by hand"
            )
        raise EditError(f"{name} is not defined in this file")
    if len(matches) > 1:
        raise EditError(
            f"{name} is defined {len(matches)} times in this file, most likely "
            "in separate #if arms; edit it by hand"
        )
    return matches[0]


def _errors(node: ts.Node) -> int:
    """How many ERROR or MISSING nodes a subtree holds."""
    count = 0
    stack = [node]
    while stack:
        current = stack.pop()
        if current.type == "ERROR" or current.is_missing:
            count += 1
            continue
        stack.extend(current.children)
    return count


def _current_params(
    src: bytes, definition: ts.Node, declarator: ts.Node
) -> list[tuple[Type, str]]:
    declarations = [c for c in definition.named_children if c.type == "declaration"]
    return _params(src, declarator, _kr_types(src, declarations))


def _parses_as(candidate: str, type_: Type, name: str) -> bool:
    """Does `candidate` read back as exactly this (type, name) parameter?"""
    fragment = f"void _f({candidate});".encode()
    tree = parse(fragment)
    if tree.root_node.has_error:
        return False

    stack = [tree.root_node]
    while stack:
        node = stack.pop()
        if node.type == "parameter_declaration":
            return _declared(fragment, node) == [(type_, name)]
        stack.extend(node.named_children)
    return False


def _render_param(type_: Type, name: str) -> str:
    """Put a declared name back into an abstract declarator.

    The inverse of _abstract, and the hard direction: the name belongs at a
    different place in `s32`, `UnkNode *`, `s32 [4]` and `void (*)(int)`.
    Rather than reimplement declarator syntax, try each position and keep the
    first that parses back to the type and name asked for, so a rendered
    parameter is verified by construction.
    """
    if type_ == "...":
        return "..."
    if not name:
        return type_
    if "(" not in type_ and "[" not in type_:
        return f"{type_}{name}" if type_.endswith("*") else f"{type_} {name}"

    for i in range(len(type_) + 1):
        candidate = type_[:i] + name + type_[i:]
        if _parses_as(candidate, type_, name):
            return candidate
    raise EditError(f"cannot place the name {name!r} in the type {type_!r}")


def _render_params(signature: list[tuple[Type, str]]) -> str:
    if not signature:
        return "(void)"
    return "(" + ", ".join(_render_param(t, n) for t, n in signature) + ")"


def _render_body(body: str) -> str:
    """A replacement body, braces included.

    Text that already carries its own braces is used verbatim, so a caller
    controls its own formatting. Anything else is wrapped and indented one
    level, which is what a caller passing a bare statement list means.
    """
    stripped = body.strip()
    if stripped.startswith("{") and stripped.endswith("}"):
        return stripped

    lines = [
        f"{_INDENT}{line}" if line.strip() else "" for line in stripped.splitlines()
    ]
    return "\n".join(["{", *lines, "}"])


def _render_comment(comment: str) -> str:
    """A replacement comment, delimiters included.

    Text that already carries `/*` and `*/` is used verbatim, so a caller
    controls its own formatting. Anything else is wrapped in the shape this
    tree writes: continuation lines indented three spaces to sit under the
    opener, and `*/` alone on the last line (214 of those against 105
    indented, and no trailing-`*/` form at all).
    """
    stripped = comment.strip()

    # A delimiter in the wrong place does not break the parse -- a stray
    # `/*` inside a comment is just text -- so nothing downstream catches
    # it, and the file ends up with a mangled header. Reject it here.
    if stripped.startswith("/*") and stripped.endswith("*/"):
        if "*/" in stripped[2:-2]:
            raise EditError(
                "the comment closes early: `*/` appears inside its own "
                "delimiters; the file has not been written"
            )
        return stripped
    if stripped.startswith("/*"):
        raise EditError(
            "the comment opens with `/*` but does not close with `*/`; "
            "the file has not been written"
        )
    if "/*" in stripped or "*/" in stripped:
        raise EditError(
            "the comment text carries a `/*` or `*/` of its own; pass the "
            "whole comment including its delimiters instead, or text with "
            "none; the file has not been written"
        )

    lines = stripped.splitlines() or [""]
    if len(lines) == 1:
        return f"/* {lines[0]} */"

    rest = [f"   {line.strip()}" if line.strip() else "" for line in lines[1:]]
    return "\n".join([f"/* {lines[0].strip()}", *rest, "*/"])


@contextlib.contextmanager
def _locked(path: pathlib.Path) -> Generator[IO[bytes], None, None]:
    """Hold an exclusive flock on the file itself for the whole edit.

    The lock is taken on the file being rewritten, and the rewrite is done
    in place rather than through a temporary file and os.replace, so the
    locked inode is the one that changes. Writing a replacement would leave
    the lock guarding an inode nothing reads afterwards.

    Two consequences worth knowing. flock is advisory, so a concurrent
    session's editor or any tool that does not take the lock is not held
    back by it. And an in-place rewrite is not atomic, so a process killed
    mid-write leaves a truncated file; the caller gets the file's previous
    contents back on any error before the write begins.
    """
    with open(path, "r+b") as handle:
        fcntl.flock(handle.fileno(), fcntl.LOCK_EX)
        try:
            yield handle
        finally:
            fcntl.flock(handle.fileno(), fcntl.LOCK_UN)


def edit_function(
    filepath: str | pathlib.Path,
    name: str,
    body: str | None = None,
    signature: list[tuple[Type, str]] | None = None,
    comment: str | None = None,
    output: str | pathlib.Path | None = None,
) -> Function:
    """Rewrite a function's body, its parameter list, or both.

    `name` says which function to edit -- a file holds many. A parameter
    left None keeps what the file already has, so passing none of them reads
    the function and writes nothing.

    `comment` replaces the comment block immediately above the definition.
    It has to be there already: a function with no header comment raises
    rather than growing one, because where a new comment belongs -- and what
    blank line goes with it -- is not something this can infer.

    `output` writes the result somewhere else instead, leaving the input
    untouched -- what a sweep over many units wants, since it can read the
    committed file and lay the edit down in a working copy. Missing parent
    directories are created. The returned spans then describe the output.

    The whole read-modify-write runs under an exclusive flock on the input,
    because this repo is worked on by concurrent sessions. Returns the
    function as it reads after the edit, body included: every span has
    moved, so one read before the edit is no longer safe to use.

    Two things it does not do. A rewritten parameter list is rendered on one
    line, so a signature that the file wraps over several lines is reflowed
    unless the rendered text matches byte for byte; nothing is written when
    both arguments render to what is already there. And renaming a parameter
    rewrites only the declaration -- the body still refers to the old name,
    which is the caller's to fix.
    """
    path = pathlib.Path(filepath)
    target = path if output is None else pathlib.Path(output)
    in_place = target.resolve() == path.resolve()

    with _locked(path) as handle:
        src = handle.read()
        definition, declarator, plist, body_node = _find_definition(src, name)

        # Splice from the right, so the earlier span stays valid. An
        # argument that asks for what the file already has is left alone:
        # the signature is compared as parsed parameters rather than as
        # text, so re-applying one unchanged does not reflow a parameter
        # list that the file wraps over several lines.
        edited = src
        if body is not None:
            rendered = _render_body(body).encode()
            if rendered != src[body_node.start_byte : body_node.end_byte]:
                edited = (
                    edited[: body_node.start_byte]
                    + rendered
                    + edited[body_node.end_byte :]
                )
        if signature is not None and signature != _current_params(
            src, definition, declarator
        ):
            edited = (
                edited[: plist.start_byte]
                + _render_params(signature).encode()
                + edited[plist.end_byte :]
            )
        if comment is not None:
            span = _comment_span(src, definition)
            if span is None:
                raise EditError(
                    f"{name} has no comment immediately above it to update; "
                    "the file has not been written"
                )
            rendered = _render_comment(comment).encode()
            if rendered != src[span[0] : span[1]]:
                edited = edited[: span[0]] + rendered + edited[span[1] :]

        # An in-place edit that changes nothing leaves the file alone, mtime
        # included. An edit to another path still has to produce that file:
        # a sweep expects its output to exist whether or not the input
        # already said what was asked for.
        if edited == src and in_place:
            return _described(src, name, path)

        # Refuse to write a file the edit has broken. tree-sitter recovers
        # from bad input rather than failing, so finding the function again
        # proves nothing -- what matters is that the edit introduced no new
        # ERROR nodes inside it. Counting rather than testing has_error
        # keeps the already-erroring shapes this dialect produces editable.
        try:
            _, _, _, after = _find_definition(edited, name)
        except EditError as exc:
            raise EditError(
                f"{name} no longer parses as a function after the edit; "
                f"the file has not been written ({exc})"
            ) from exc

        if _errors(after.parent or after) > _errors(definition):
            raise EditError(
                f"the replacement would add a syntax error to {name}; "
                "the file has not been written"
            )

        if in_place:
            # Rewrite the locked inode itself; see _locked on why this is
            # not a temp-file-and-rename.
            handle.seek(0)
            handle.write(edited)
            handle.truncate()
            handle.flush()
        else:
            # A different file is not the one under lock, so nothing is
            # guarding it -- write it atomically instead, and a reader sees
            # either the old contents or the new, never a partial file.
            target.parent.mkdir(parents=True, exist_ok=True)
            temporary = target.with_name(f".{target.name}.cfuncs-tmp")
            temporary.write_bytes(edited)
            os.replace(temporary, target)

    return _described(edited, name, target)


def _described(src: bytes, name: str, path: pathlib.Path) -> Function:
    """The edited function as the written bytes now read."""
    for function in iter_functions(src, include_body=True,
                                   include_comment=True):
        if function.name == name:
            return function
    raise EditError(f"{name} is no longer in {path} after the edit")
