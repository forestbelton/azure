import pathlib

from mcp.server import MCPServer
from mcp.server.mcpserver.exceptions import ToolError

import func

ROOT = pathlib.Path(__file__).resolve().parents[2]

mcp = MCPServer("cfuncs")


@mcp.tool()
def list_functions(filepath: str) -> list[func.Function]:
    """List the function definitions in a C file, with exact source spans."""
    path = (ROOT / filepath).resolve()
    if not path.is_relative_to(ROOT):
        raise ToolError(f"{filepath} is outside the repository")
    if not path.is_file():
        raise ToolError(f"{filepath} does not exist")
    return func.list_functions(path)


if __name__ == "__main__":
    mcp.run(transport="stdio")
