import json

with open("slus_006.14.strings.json", "r") as f:
    entries = json.load(f)

for entry in entries:
    offset = 0x800 + entry["offset"]
    bin_offset = offset + entry["len"] + 1
    print(f"      - [{hex(offset)}, shiftjis]")
    print(f"      - [{hex(bin_offset)}, bin]")
