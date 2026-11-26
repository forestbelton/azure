"""THE single source of truth for "what is in this C file".

Every ad-hoc regex for finding a function definition or counting
`INCLUDE_ASM' stubs in this project has been wrong at least once, and each
time it produced a PLAUSIBLE WRONG NUMBER rather than an error:

  * a definition finder matching `^[A-Za-z_][^\\n;]*\\bNAME\\s*\\(' matched a
    park's own FORWARD DECLARATION inside its header comment, so permimport
    sliced the file from the wrong offset and cc1psx failed on prose;
  * an `awk' scan for `^[a-zA-Z_].*\\(' named a function 400 lines too early,
    so three measurements in a row were about a body that did not contain the
    edit, and only `make containers' caught it;
  * an `INCLUDE_ASM' pattern expecting THREE comma-separated fields where the
    real macro takes TWO reported "3 written" per unit when the truth was 8,
    7, 6 and 5 -- which nearly discarded twelve landed bodies;
  * `grep -c INCLUDE_ASM' counted a COMMENT that mentioned the macro and
    reported one stub for a unit with none.

So: ask this module, do not write the regex again.

Function enumeration is tree-sitter's, via `tools/c/func.py'. Stub
enumeration is here, because `INCLUDE_ASM' is a macro and no C parser sees it
as a function -- but it is done on COMMENT-STRIPPED text, which is the whole
reason the grep above was wrong.

REQUIRES `uv run' (tree_sitter lives in the project venv, not in the system
python), e.g. `uv run python tools/decomp/dngland.py ...'.
"""
import os, re, sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(PROJ, "tools", "c"))

try:
    import func as _func
except ModuleNotFoundError as e:                      # pragma: no cover
    raise SystemExit(
        "cfuncs: %s -- run this under `uv run' (tree_sitter is in the project "
        "venv, not the system python)." % e)


def strip_comments(text):
    """Blank out /* */ and // comments, PRESERVING length and line count.

    Offsets and line numbers therefore stay valid against the original text,
    which is what lets a caller use this for matching and then slice the real
    file.
    """
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == '"' or c == "'":                       # skip string literals
            q = c; i += 1
            while i < n and text[i] != q:
                i += 2 if text[i] == "\\" else 1
            i += 1
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != "\n":
                    out[k] = " "
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            for k in range(i, j):
                out[k] = " "
            i = j
        else:
            i += 1
    return "".join(out)


_STUB = re.compile(r'INCLUDE_ASM\s*\(\s*"[^"]*"\s*,\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)')


def functions(path):
    """-> list of tools/c/func.Function for every DEFINITION in the file."""
    return _func.list_functions(path)


def names(path):
    """-> set of defined function names."""
    return {f.name for f in functions(path)}


def find(path, name):
    """-> the Function named `name', or None. Never a forward declaration."""
    for f in functions(path):
        if f.name == name:
            return f
    return None


def stubs(path):
    """-> list of INCLUDE_ASM'd names, in file order, comments excluded."""
    text = io_read(path)
    return _STUB.findall(strip_comments(text))


def io_read(path):
    import io
    return io.open(path, encoding="utf-8", errors="replace").read()


def written(copy_path, csrc_path):
    """Names that are stubs in `csrc_path' and DEFINED in `copy_path'.

    The recovery primitive: what an agent's working copy has landed that the
    committed unit has not. Asserts both sides parsed, because an empty set
    from a silent parse failure is indistinguishable from "nothing to do" --
    and that is exactly how twelve bodies were nearly lost.
    """
    cs = set(stubs(csrc_path))
    assert cs or not stubs(csrc_path), "no stubs parsed in %s" % csrc_path
    defined = names(copy_path)
    assert defined, "no definitions parsed in %s" % copy_path
    return sorted(cs & defined)


def main():
    if len(sys.argv) < 2:
        raise SystemExit("usage: cfuncs.py FILE.c [NAME] | --stubs FILE.c "
                         "| --written COPY.c CSRC.c")
    if sys.argv[1] == "--stubs":
        for n in stubs(sys.argv[2]):
            print(n)
    elif sys.argv[1] == "--written":
        for n in written(sys.argv[2], sys.argv[3]):
            print(n)
    elif len(sys.argv) == 3:
        f = find(sys.argv[1], sys.argv[2])
        print("not found" if f is None else
              "%s: ret=%r params=%d lines %d-%d bytes %d-%d"
              % (f.name, f.ret_type, len(f.params), f.line_start, f.line_end,
                 f.byte_start, f.byte_end))
    else:
        for f in functions(sys.argv[1]):
            print("%-28s %-10r %d params  lines %d-%d"
                  % (f.name, f.ret_type, len(f.params), f.line_start, f.line_end))


if __name__ == "__main__":
    main()
