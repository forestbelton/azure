import argparse

from azure.commands import (
    convert_shiftjis,
    # create_string_yaml,
    extract_symbol_info,
    find_shiftjis_strings,
    process_bincue,
    process_symbol_addrs,
    psylib,
)

ALL_PARSERS = {
    "shiftjis": convert_shiftjis.setup_parser,
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
