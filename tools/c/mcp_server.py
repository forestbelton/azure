import pathlib

from mcp.server import MCPServer
from mcp.server.mcpserver.exceptions import ToolError

import func

ROOT = pathlib.Path(__file__).resolve().parents[2]

mcp = MCPServer("cfuncs")


def _resolve(filepath: str, must_exist: bool = True) -> pathlib.Path:
    """A path inside the repository, or a ToolError saying why not."""
    path = (ROOT / filepath).resolve()
    if not path.is_relative_to(ROOT):
        raise ToolError(f"{filepath} is outside the repository")
    if must_exist and not path.is_file():
        raise ToolError(f"{filepath} does not exist")
    return path


@mcp.tool()
def describe_functions(filepath: str, include_body: bool = False,
                       include_comment: bool = False) -> list[func.Function]:
    """List the function definitions in a C file, with exact source spans.

    This is half the answer to "what is in this file": the other half is the
    INCLUDE_ASM stubs, which no C parser sees as functions. Call
    describe_stubs for those.

    `include_body` fills in each function's body. It is off by default
    because bodies are most of the bytes -- a whole file with them is around
    four times the size -- and a span is enough to slice the text.
    `include_comment` fills in the comment block above each definition.
    """
    return func.describe_functions(_resolve(filepath),
                                   include_body=include_body,
                                   include_comment=include_comment)


@mcp.tool()
def describe_function(filepath: str, name: str, include_body: bool = False,
                      include_comment: bool = False) -> func.Function | None:
    """One named function's signature and span, or null if it is not defined.

    Never matches a forward declaration or a mention in a comment. A name
    that is an INCLUDE_ASM stub is not defined and returns null; describe_stubs
    reports those.

    `include_body` fills in the body, and `include_comment` the comment block
    immediately above the definition -- in this tree, the header recording
    how the body was derived. Both are off by default.
    """
    return func.describe_function(_resolve(filepath), name,
                                  include_body=include_body,
                                  include_comment=include_comment)


@mcp.tool()
def describe_stubs(filepath: str) -> list[func.Stub]:
    """List the INCLUDE_ASM stubs in a C file -- the functions not yet landed.

    Found structurally, so a comment mentioning INCLUDE_ASM and a mention
    inside a string literal are both excluded; counting the text instead once
    reported one stub for a unit that had none. INCLUDE_RODATA is not
    included, because a stub stands for a function.
    """
    return func.describe_stubs(_resolve(filepath))


@mcp.tool()
def edit_function(
    filepath: str,
    name: str,
    body: str = "",
    signature: list[tuple[func.Type, str]] | None = None,
    comment: str = "",
    output: str = "",
) -> func.Function:
    """Rewrite a named function's body, its parameter list, or both.

    Leave body out to keep the body already in the file, and leave signature
    out to keep its parameters; the function is identified by name, since a
    file holds many. A body that carries its own braces is used verbatim, so
    the caller controls its formatting; a bare statement list is wrapped and
    indented one level. Each signature entry is a (type, name) pair using
    the same spelling describe_functions reports, so `["UnkNode *", "n"]` and
    `["void (*)(int)", "cb"]` both read back unchanged.

    `comment` replaces the comment block immediately above the definition.
    One has to be there already -- a function with no header comment is an
    error rather than gaining one, since where a new comment belongs cannot
    be inferred. Text carrying its own `/*` and `*/` is used verbatim;
    anything else is wrapped in this tree's shape, continuation lines
    indented three spaces and `*/` alone on the last line.

    `output` writes the result to a different path and leaves the input
    alone, which is what a sweep wants: read the committed unit, lay the
    edit down in a working copy. Parent directories are created.

    The edit runs under an exclusive lock on the input. Nothing is written if
    the replacement would add a syntax error to the function, or if an
    in-place edit was asked for what is already there. Returns the function
    as it reads afterwards, body included.
    """
    # `body`, `comment` and `output` are `str = ""` rather than
    # `str | None = None` on
    # purpose. The SDK pre-parses any string argument whose annotation is not
    # exactly `str`, to cope with clients that send lists and objects
    # JSON-encoded. A C body is braced, so an empty one -- `{}` -- is valid
    # JSON and arrives as a dict, failing validation before this function is
    # entered. An annotation of plain `str` is skipped by that pass.
    # `signature` keeps its optional annotation because there the pre-parsing
    # is wanted.
    path = _resolve(filepath)
    target = _resolve(output, must_exist=False) if output else None
    try:
        return func.edit_function(path, name, body=body or None,
                                  signature=signature,
                                  comment=comment or None, output=target)
    except func.EditError as exc:
        raise ToolError(str(exc)) from exc


if __name__ == "__main__":
    mcp.run(transport="stdio")
