"""Check that psyq/ holds the correct SDK contents."""

import hashlib
import pathlib
import re
import sys

PSYQ_PREFIX = "psyq"

RAW_HEADERS = """
include/abs.h          9cf0dad59d51391888da798c8fc6463f2d884e097566a4810ecc167eac8facad
include/asm.h          d23cc963d29abcb00164de4676f42e9ea591eeeed66e36b7d0c9be07bd85d80a
include/assert.h       d67caeaaf360af60d671e3e3c8d6ab2c6d162b4d4526f0af6c40961b21478dda
include/autopad.h      e3eb07530e9b488ea9d911836a2ea05a22f1af74fdd54079360ff3715842777c
include/convert.h      653a1e6f0389bb09c59b8c3280721c164d9679a1a8778a6cb312db41e61ee28a
include/ctype.h        0a15eb01c5857e390db7037db24e6b835527c4f0a4907cbe25e729336e622e6c
include/fs.h           894d7e4adcb8fa051dd72748453d5067b851986cae7973f16dabbf06a129f7c5
include/gtemac.h       15b007e98f3b004ebaf396aa11f87f4c03728e244754feb0ed33e624c2b80fde
include/gtenom.h       0149f0a4893becfc4088db918f64a1e4cb71fd422a2d3072c8de0f7f27fcfc56
include/gtereg.h       2e99c414d735125cbdaf61c3d17403dc66131f74aea1eedb7651e69898165938
include/inline_a.h     f4d00141f3eb606d8085e8a351e739fea47ab8f9e92bc00cc8b14bb7b0a26381
include/inline_c.h     2910a9972feac80b817d02b4d3a93a761d9ca47f0de16070b5bd7674fab9a71c
include/inline_o.h     715a9f5b1e411116f279a3f33060416b1cc1e29efd195759d2537dc3281caafc
include/kernel.h       b01a58a56eeb94e756dfea9b8af9ab639f7b99be17feccf4c00c650a992dfdff
include/libcd.h        0ae8032a73e6125acfac5d37d0becb96da6bde2adae0e047801989ae6efd2d2d
include/libcomb.h      58a749445fabd2e1d1732d5df721768e3ca1392811527e96faf9c4d52d574cd8
include/libds.h        785ca83f3deb172845065024f47c71f8708e984072285fca55628f4c4f09d01e
include/libetc.h       f5220d2a1c7dfe31164b7832d82e643571eeef3be0b419d6f9dbe5e5194f5cec
include/libgpu.h       bd13d8375fa5f13117aca503ae1af0a236c5f0c90a1105ea291831cd5c36c162
include/libgs.h        1c8146f93c1f2d8d4d1405e2ad562c6907fbbdd1d6452c9d42f90451aa88bb76
include/libgte.h       21cf985e3db9b9ea20f7f8ce75ad79683496b20e16e235e908a2652439253646
include/libgun.h       76da785dde36fb529b07f73955c2860f2535b7fa5454abb6a3fccb11fc66c59f
include/libmath.h      59a647e8f99c68099d827cc4987e0b83033c7ff64868f56f0c510997313712d0
include/libmcrd.h      cd2a0bf3f5f14d15213d08338e9a9b8ee2374cc74e4e46bf0ab5048d0fdb131f
include/libpress.h     1fb67b2657b4447d453257f66beb2c89dce9b2f9dab9149c7e72e75e2fd542f8
include/libsio.h       330f0beeaa09bf92ca355cd187121071c6b12a6bed6122329a4eb3cfb72d237f
include/libsn.h        713aa3dcf5bcb5fb75ac76fb731fcf4f07c2f54e89bcfeead1ca13fafc00ced9
include/libsnd.h       daa9e74007762c838f15a43df273b83878ad440231fe7e4dca6e9acce4a0a6ac
include/libspu.h       db92016eb59dbf69752ee526ec5db75a2fa9964378602a38be71849934112bc1
include/libtap.h       fb6d85c580d6eac41ae6494915f99be058774ccff26a546220ad3c80138873ed
include/limits.h       2b4e52bf4ba0c63d92c82d90a0728b4e3b5b84b080b47390b5c5eb32da4ad05f
include/malloc.h       0875ee9a0903415df6dfd17529359e07c7ea9fbf6a193221e08996bd59b9447b
include/memory.h       3edf0b78728c4bced63ab05662ca76cacfe3ea752fbca2c472798aec2da3e86c
include/qsort.h        5a38cd40323c66be034b79719ff8565f2343fd4797d91ba551c506a2f799c098
include/r3000.h        ec3e7720f3683510b475c9d864b5e6ac041d766344baeaef12442e48bf39208c
include/rand.h         8ba070220576b5736dc5c6116ba3cd70eef4552f08b930406245ba819775d326
include/romio.h        17f18998c6513ca5e4b29e8c15b71fdc7fe5233600ee4b805f32662f8d92548e
include/setjmp.h       3c735fa407417feaeda88532a85d3ffbdcd305c593bf597c9680a0309570e020
include/stdarg.h       52c7da45e69d39e650d68369df349fc0be27552f07965f29239ddd22ded74004
include/stddef.h       1135b7e17d3b2052e7e16bfec44498ed8b3af6733b0e3aa3e58e1fe5d0277795
include/stdio.h        63f65ce55b2e4ea004dd05af7ec40daaba10f6c4ef22180622340ebdc0b3c59d
include/stdlib.h       687ae84b80b17f9fa17e8e5bea5e15bfc845594b679a83f90724fbb25b5f95f7
include/string.h       b07eaa54dd34dc80c965a48740a13959c74f32f1c0f4bad4dca9c28371519e3c
include/strings.h      58777cfd47c8f541d62d51ec69f0b03e64bb9743886c7ce4c91774ff252d4501
include/sys/errno.h    a113437201792e39574c503209435ac25ebc26093817f8fd1f86ea3590460b60
include/sys/fcntl.h    b03139754b990d8232be5f94259063937160e2dddd932a4c6b36767fadc7b750
include/sys/file.h     e522dec5e932a8e558ddd3cc7bec842a0d4473f7969f3f14187de9a78f2ea680
include/sys/ioctl.h    b10c26a7ac06ea541f5ebd508c3c6309d1cc2f94491912a7f53035f554cb74fc
include/sys/types.h    085c69c336e6d4af8bc3468a942125803d3173064f150e90be5226d390445413
"""


def parse(raw: str) -> dict[str, str]:
    return dict(
        [tuple(re.split(" +", line)) for line in raw.strip().split("\n")]  # type: ignore
    )


HEADERS = parse(RAW_HEADERS)


def sha256(path: pathlib.Path) -> str:
    hash = hashlib.sha256()
    with open(path, "rb") as f:
        chunk = f.read(512)
        while len(chunk) != 0:
            hash.update(chunk)
            chunk = f.read(512)
    return hash.hexdigest()


def check(manifest: dict[str, str], label: str) -> bool:
    ok = True
    for raw_path, digest in manifest.items():
        path = pathlib.Path(PSYQ_PREFIX, raw_path)
        if not path.exists() or not path.is_file():
            ok = False
            print(f"missing: {path}", file=sys.stderr)
            continue
        actual = sha256(path)
        if actual != digest:
            ok = False
            print(
                f"bad shasum: file={path} actual={actual} expected={digest}",
                file=sys.stderr,
            )
    if ok:
        print(f"{label}: OK ({len(manifest)} files)")
    return ok


def main() -> None:
    ok = check(HEADERS, "headers")
    if not ok:
        sys.exit(1)


if __name__ == "__main__":
    main()
