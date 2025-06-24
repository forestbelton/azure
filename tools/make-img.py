import argparse
import dataclasses
import os
import struct
from typing import BinaryIO

import PIL.Image

RGBA = tuple[int, int, int, int]
CLUT = list[RGBA]


@dataclasses.dataclass
class Rect:
    x: int
    y: int
    w: int
    h: int


def get_clut_rect(param_2: int, param_3: int, param_4: int) -> Rect:
    iVar1 = param_2 + -0x7000 >> 6
    if param_4 & 1 != 0:
        raise Exception("CLUT does not have rect")
    return Rect(
        x=(iVar1 & 0x40) * 2 + (param_2 & 0x3F) * 16,
        y=(iVar1 & 0x3F) + 0x1C0,
        w=param_3 << 4,
        h=1,
    )


def decompress(f: BinaryIO) -> bytes:
    src = bytearray()
    dst = bytearray()

    bit_buffer = 0
    bit_buffer_idx = 1
    src_idx = 0
    dst_idx = 0

    def get_src(idx: int) -> int:
        if idx >= len(src):
            src.extend(f.read(idx - len(src) + 1))
        assert idx < len(src)
        return src[idx]

    def set_dst(idx: int, x: int) -> None:
        while idx >= len(dst):
            dst.append(0)
        dst[idx] = x & 0xFF

    while True:
        while True:
            bit_buffer_idx -= 1
            src_ptr_idx = src_idx
            if bit_buffer_idx == 0:
                bit_buffer = get_src(src_idx)
                src_ptr_idx = src_idx + 1
                bit_buffer_idx = 8
            if (bit_buffer & 1) != 0:
                break
            bit_buffer >>= 1
            src_idx = src_ptr_idx + 1
            set_dst(dst_idx, get_src(src_ptr_idx))
            dst_idx += 1
        bit_buffer_idx -= 1
        bit_buffer >>= 1
        if bit_buffer_idx == 0:
            bit_buffer = get_src(src_ptr_idx)
            src_ptr_idx += 1
            bit_buffer_idx = 8
        if (bit_buffer & 1) == 0:
            uVar2 = (get_src(src_ptr_idx) << 8) | get_src(src_ptr_idx + 1)
            bit_buffer = bit_buffer >> 1
            if uVar2 == 0:
                return bytes(dst[:dst_idx])
            src_idx = src_ptr_idx + 2
            if (get_src(src_ptr_idx + 1) & 0xF) == 0:
                bVar1 = get_src(src_idx)
                src_idx = src_ptr_idx + 3
                length = bVar1 + 1
            else:
                length = (uVar2 & 0xF) + 2
            back_ref_offset = uVar2 >> 4
        else:
            bit_buffer_idx -= 1
            bit_buffer >>= 1
            if bit_buffer_idx == 0:
                bit_buffer = get_src(src_ptr_idx)
                src_ptr_idx += 1
                bit_buffer_idx = 8
            bit_buffer_idx -= 1
            back_ref_offset = bit_buffer >> 1
            if bit_buffer_idx == 0:
                back_ref_offset = get_src(src_ptr_idx)
                src_ptr_idx += 1
                bit_buffer_idx = 8
            length = (bit_buffer & 1) * 2 + 2 + (back_ref_offset & 1)
            bit_buffer = back_ref_offset >> 1
            back_ref_offset = get_src(src_ptr_idx)
            src_idx = src_ptr_idx + 1
            if back_ref_offset == 0:
                back_ref_offset = 0x100
        src_ptr_idx = dst_idx - back_ref_offset
        while length != 0:
            bVar1 = dst[src_ptr_idx]
            src_ptr_idx += 1
            set_dst(dst_idx, bVar1)
            dst_idx += 1
            length -= 1


# Assumes that the AZ-IMG only has 1 CLUT and 1 texture
def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("infile")
    args = parser.parse_args()
    with open(args.infile, "rb") as f:
        tex_rect, clut, image = parse_image(f)
    assert len(image) == tex_rect.w * 2 * tex_rect.h
    img = draw_image(tex_rect, clut, image, 4)
    img.save(f"{args.infile}.png")


def parse_image(f: BinaryIO) -> tuple[Rect, CLUT, bytes]:
    base_offset = f.tell()
    img_hdr_offset = f.tell()
    img_ty, img_hdr_len = struct.unpack("<HH", f.read(0x4))
    tex_rect = None
    clut = None
    raw_image = None
    while img_ty != 0:
        print(f"ty={img_ty:02x}")
        match img_ty:
            # Compressed texture
            case 1:
                tex_offset, tex_x, tex_y, tex_w, tex_h = struct.unpack(
                    "<IHHHH", f.read(12)
                )
                tex_rect = Rect(x=tex_x, y=tex_y, w=tex_w, h=tex_h)
                print(f"{tex_rect=}")
                f.seek(base_offset + tex_offset, os.SEEK_SET)
                raw_image = decompress(f)
                assert len(raw_image) == tex_rect.w * tex_rect.h * 2
            # CLUT
            case 2:
                clut_offset, clut_param1, clut_num_entries, clut_param2 = struct.unpack(
                    "<IHHH", f.read(10)
                )
                clut_rect = get_clut_rect(clut_param1, clut_num_entries, clut_param2)
                print(f"{clut_rect=}")
                f.seek(base_offset + clut_offset, os.SEEK_SET)
                raw_clut = struct.unpack(f"<{clut_rect.w}H", f.read(clut_rect.w * 2))
                clut = [raw_clut_to_rgba(entry) for entry in raw_clut]
            # Uncompressed texture
            case 5:
                tex_offset, tex_x, tex_y, tex_w, tex_h = struct.unpack(
                    "<IHHHH", f.read(12)
                )
                tex_rect = Rect(x=tex_x, y=tex_y, w=tex_w, h=tex_h)
                print(f"{tex_rect=}")
                f.seek(base_offset + tex_offset, os.SEEK_SET)
                raw_image = f.read(tex_rect.w * tex_rect.h * 2)
                assert len(raw_image) == tex_rect.w * tex_rect.h * 2
            # CLUT (with transform?)
            case 6:
                clut_offset, clut_param1, clut_num_entries, clut_param2 = struct.unpack(
                    "<IHHH", f.read(10)
                )
                clut_rect = get_clut_rect(
                    clut_param1, clut_num_entries, clut_param2 | 2
                )
                print(f"{clut_rect=}")
                f.seek(base_offset + clut_offset, os.SEEK_SET)
                raw_clut = struct.unpack(f"<{clut_rect.w}H", f.read(clut_rect.w * 2))
                clut = [raw_clut_to_rgba(entry) for entry in raw_clut]
            # TODO: types 3, 4, 7, 8, 9
            case _:
                raise Exception(f"unsupported image type {img_ty}")
        img_hdr_offset += img_hdr_len
        f.seek(img_hdr_offset, os.SEEK_SET)
        img_ty, img_hdr_len = struct.unpack("<HH", f.read(0x4))
    if tex_rect is None or raw_image is None:
        raise Exception(f"missing image data")
    if clut is None:
        raise Exception(f"missing CLUT data")
    return tex_rect, clut, raw_image


def raw_clut_to_rgba(raw: int) -> RGBA:
    r = ((raw >> 0) & 0x1F) << 3
    g = ((raw >> 5) & 0x1F) << 3
    b = ((raw >> 10) & 0x1F) << 3
    is_semitrans = (raw >> 15) & 1
    a = 0xFF
    if not is_semitrans and r == 0 and g == 0 and b == 0:
        a = 0
    return (r, g, b, a)


def draw_image(
    tex_rect: Rect, clut: list[RGBA], raw_image: bytes, depth: int = 8
) -> PIL.Image.Image:
    match depth:
        case 4:
            width = tex_rect.w * 4
        case 8:
            width = tex_rect.w * 2
        case _:
            raise Exception(f"unsupported color depth {depth}")
    img = PIL.Image.new("RGBA", (width, tex_rect.h))
    idx = 0
    for y in range(tex_rect.h):
        for x in range(width):
            match depth:
                case 8:
                    color_idx = raw_image[idx]
                    idx += 1
                case 4:
                    if x % 2 == 0:
                        color_idx = raw_image[idx] & 0xF
                    else:
                        color_idx = raw_image[idx] >> 4
                        idx += 1
            img.putpixel((x, y), clut[color_idx])
    return img


if __name__ == "__main__":
    main()
