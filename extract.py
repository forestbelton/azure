# TODO: Turn into `azure` command
import argparse
import pathlib
import sys
from typing import BinaryIO

import hashlib

from pydantic import dataclasses
import yaml

from azure.commands import bincue


@dataclasses.dataclass
class SpecFile:
    name: str
    shasum: str
    sector: int
    size: int


@dataclasses.dataclass
class Spec:
    infile: str
    shasum: str
    outdir: str
    files: list[SpecFile]


def extract_file(f: BinaryIO, start_sector: int, file_size: int) -> bytes:
    data = bytearray()
    sector_index = start_sector
    while len(data) < file_size:
        sector = bincue.extract_sector(f, sector_index)
        bytes_in_chunk = min(len(sector.data), file_size - len(data))
        data += sector.data[:bytes_in_chunk]
        sector_index += 1
    return data


def extract_file_to(
    inf: BinaryIO, outfile: str, start_sector: int, file_size: int
) -> None:
    data = extract_file(inf, start_sector, file_size)
    with open(outfile, "wb") as f:
        f.write(data)


def get_file_shasum(filename: str) -> str:
    hash = hashlib.sha1()
    with open(filename, "rb") as f:
        for chunk in iter(lambda: f.read(128 * hash.block_size), b""):
            hash.update(chunk)
    return hash.hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("spec")
    args = parser.parse_args()
    with open(args.spec, "r") as f:
        raw_spec = yaml.full_load(f)
    spec = Spec(**raw_spec)
    infile_shasum = get_file_shasum(spec.infile)
    if spec.shasum != infile_shasum:
        print(f"ERROR: input file does not have expected shasum", file=sys.stderr)
        print(f"actual:   {infile_shasum}", file=sys.stderr)
        print(f"expected: {spec.shasum}", file=sys.stderr)
        sys.exit(1)
    outdir = pathlib.Path(spec.outdir)
    with open(spec.infile, "rb") as f:
        for spec_file in spec.files:
            spec_outfile = outdir / spec_file.name
            spec_outfile.parent.mkdir(parents=True, exist_ok=True)
            extract_file_to(f, spec_outfile, spec_file.sector, spec_file.size)
            outfile_shasum = get_file_shasum(spec_outfile)
            if spec_file.shasum != outfile_shasum:
                print(
                    f"ERROR: extracted file {spec_file.name} does not have expected shasum",
                    file=sys.stderr,
                )
                print(f"actual:   {outfile_shasum}", file=sys.stderr)
                print(f"expected: {spec_file.shasum}", file=sys.stderr)
                sys.exit(1)


if __name__ == "__main__":
    main()
