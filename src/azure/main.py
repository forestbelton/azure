import argparse

from azure.commands import (
    bincue,
    extract,
    findstr,
    lib,
    shiftjis,
    symbols,
)

ALL_PARSERS = {
    "bincue": bincue.setup_parser,
    "extract": extract.setup_parser,
    "findstr": findstr.setup_parser,
    "lib": lib.setup_parser,
    "shiftjis": shiftjis.setup_parser,
    "symbols": symbols.setup_parser,
}


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser("azure")
    subparsers = parser.add_subparsers(required=True)
    for subcommand, setup_func in ALL_PARSERS.items():
        cmd_parser = setup_func(subcommand)
        subparsers.add_parser(
            subcommand,
            parents=[cmd_parser],
            description=cmd_parser.description,
            conflict_handler="resolve",
        )
    return parser


def main() -> None:
    parser = build_parser()
    args = parser.parse_args()
    args.main(args)


if __name__ == "__main__":
    main()
