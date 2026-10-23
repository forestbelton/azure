import hashlib
import pathlib
import re
import sys

PSYQ_PREFIX = "psyq"

RAW_MANIFEST = f"""
aspsx.exe        7f928fa89e143facd75737822b08bb63fb41a06415ba943393db727aa398efc8
aspsx-2.67.exe   44103a7e64432095d0795e434232aa8deabeade393063f56e00ce0d329382df7
cc1plpsx.exe     855bf8e3194980e1cd5eb9d6ce763ffca5dcbc3af899ecfa9d4786dd024cb164
cc1psx.exe       17b0d9dc6a80b2aaec90f912bb442321ec8bc5cecaf7071f5bd187e1d6525d49
ccpsx.exe        7ea2ea5e16f8847f6ccdce229f74d4cd6188d7aefdd7a035200d27b524962aa5
cpppsx.exe       2eef12fc06c6f02844e2679d3fb42dab88886b84a1a7ca1fb01cc713c466c78d
lib/2mbyte.obj   b4ed8b2e76bc841f7a81132906d846877aefa8f6233fb738600c8b1ebd032a46
lib/8mbyte.obj   0c0ccb91c6d059a88b5ec5ac8f34c65a717c636a911247c31faa0df4ae2e599e
lib/autopad.obj  fab3b9d89065f18f5c6a76955958dc77b5c99b50d098bfb75c692b7bcec53bf0
lib/libapi.lib   3c082a4fd4b00c5b191b717a96035e5453b7db2f5b906b2bd68e5fca6350031d
lib/libc2.lib    3d6c0464728e014cb341a8eae337303c7c03e0dcd8c897a2549ef16e1e231c46
lib/libcard.lib  d398c47818824ce96c280ba9ae7a6cdf7bab5386c82292566d1caf723a540764
lib/libcd.lib    db4e1a716b3b49a0776fb85c7802d839e5cae4acadaeaf3b16621aa5c61ad79d
lib/libc.lib     14847db1c85806630867c1be95e7440474fef47997f398863380360695adee6e
lib/libcomb.lib  d6c8b3312e9420dcd539c3f8cdbd20fd1a2fa6d2c1e1d2b3196e23af08c14120
lib/libds.lib    0b152beb505441d5b7b7c8198ec2dcba597d935f8d8867b748e51622b160a01b
lib/libetc.lib   71a57b08c6a406148061cb5bb66aeadcbfac736b4d45d0ccc7f26635aaadae54
lib/libgpu.lib   d296e33ff3599a9962d28d59fd1883e20160f252ca85b28b8a44333199d331bf
lib/libgs.lib    d3341cd5c813f53078a72376c80d15f6f502ecc44487e2bb21ed95d585d83046
lib/libgte.lib   bec029730c9a8bc529b1980519c5cacbbbb63132c28cb524fa72ea6e4b0debea
lib/libgun.lib   12ff5443876496bf35023a04bb7c052076b82e00a1027202bb3624483bb55ba4
lib/libmath.lib  f4cf1719fb07b6daa02dba5144d8d5b09c504ad745af061ffbadc70d6f1b5bbc
lib/libmcrd.lib  8cc75e2b840944906d9ce8bf33e91e603cfd56bdcbb5fe9044ffeb7a2bc3807d
lib/libpress.lib f78f42abbd8a21ef56f0f6bdfcc4f1b9f1b4b903dfd8c3ae3add6a7f355d8a6c
lib/libsio.lib   9ff3e830d32692ac72a5d675188f770707fbb31b77c7538b496616372acc39c4
lib/libsnd.lib   4991a3c91ebb784ceea73cdc8feb2a9c10cc399af9f5b1ba0397fa89387fa715
lib/libsn.lib    1ef99108b065166613a4bf6c5b46a74bd0f584e8a570926e8e0c66497e64f547
lib/libspu.lib   ee099de953b355e580e34a83e37b44e4276cb14202afbfd2ea4ba2d4926e6d13
lib/libtap.lib   cd4c5d17246fdf301126f4f37388516bcc0e73782f11ed3f521314d23b898654
lib/noheap.obj   d429e72ab30f18df31e8df73230c30c11f1eefcfc4eb9385104ce9ebf73a0d02
lib/none3.obj    063efc868877ca84fcbe1fdc3afe9656e6c7933cb68156e08d04d35b76166899
lib/poweron.obj  f3d7ecabb8efd0ba88e9da52d3639151d85d394b9511766806f7c548fdb463e0
"""

MANIFEST: dict[str, str] = dict(
    [tuple(re.split(" +", line)) for line in RAW_MANIFEST.strip().split("\n")]  # type: ignore
)


def main() -> None:
    for raw_path, digest in MANIFEST.items():
        path = pathlib.Path(PSYQ_PREFIX, raw_path)
        if not path.exists() or not path.is_file():
            abort(f"missing file: {path}")
        with open(path, "rb") as f:
            chunk = f.read(512)
            hash = hashlib.sha256()
            while len(chunk) != 0:
                hash.update(chunk)
                chunk = f.read(512)
            hash_digest = hash.hexdigest()
            if hash_digest != digest:
                abort(
                    f"bad shasum for file: {path} (actual={hash_digest} expected={digest})"
                )
    print("OK")


def abort(msg: str) -> None:
    print(f"error: {msg}", file=sys.stderr)
    sys.exit(1)


if __name__ == "__main__":
    main()
