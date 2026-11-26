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

import dataclasses
import pathlib
from typing import Iterator

import tree_sitter as ts
import tree_sitter_c

Type = str

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
    line_start: int
    line_end: int
    byte_start: int
    byte_end: int


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
        line_start=start_row + 1,
        line_end=body.end_point[0] + 1,
        byte_start=start_byte,
        byte_end=body.end_byte,
    )


def _function(src: bytes, node: ts.Node) -> Function | None:
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
    )


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


def _recover_kr(src: bytes, parent: ts.Node) -> Iterator[Function]:
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


def iter_functions(src: bytes, tree: ts.Tree | None = None) -> Iterator[Function]:
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
            function = _function(src, node)
            if function is not None:
                found[function.byte_start] = function
                continue
        if node.has_error:
            for function in _recover_kr(src, node):
                found.setdefault(function.byte_start, function)
        stack.extend(node.named_children)

    for _, function in sorted(found.items()):
        yield function


def list_functions(filepath: str | pathlib.Path) -> list[Function]:
    """List the function definitions in a file, in source order.

    No preprocessor runs, so there is nothing to configure: the old
    `cpp_args` argument is gone, and spans index the bytes on disk.
    """
    return list(iter_functions(pathlib.Path(filepath).read_bytes()))
