# Annotate Shift-JIS strings based off a given list of offsets
# @category Strings
import json
import traceback

from ghidra.program.model.data import CharsetSettingsDefinition, StringDataType


offset_file = askFile("Offsets JSON", "Choose")
with open(str(offset_file), "r") as f:
    string_offsets = json.load(f)

base = currentProgram.getImageBase()
listing = currentProgram.getListing()
string_dt = StringDataType()

num_failures = 0

for record in string_offsets:
    addr = base.add(record["offset"])
    try:
        data = listing.createData(addr, string_dt, record["len"])
        CharsetSettingsDefinition.CHARSET.setCharset(data, "Shift_JIS")
    except:
        print("Failed to create string at " + str(addr))
        traceback.print_exc()
        num_failures += 1

print(
    "Failed to create {failures}/{total} strings ({pct}%)".format(
        failures=num_failures,
        total=len(string_offsets),
        pct=round(float(num_failures) / len(string_offsets) * 100, 2),
    )
)
