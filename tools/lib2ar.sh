#!/bin/bash
# Convert a PSY-Q library to the GNU AR format for easier introspection
# TODO: Convert this to Python
set -euo pipefail

# Override these with the correct paths for your system
AR=${AR:-$HOME/scratch/mipsel-linux-gnu/bin/mipsel-linux-gnu-ar}
AZURE_PATH=${AZURE_PATH:-$HOME/projects/azure}
PSYQ_OBJ_PARSER=${PSYQ_OBJ_PARSER:-$HOME/scratch/pcsx-redux/psyq-obj-parser}

CWD=$PWD
LIB=$(basename $1 .LIB)

# MacOS `realpath` doesn't support `--relative-to`; on this platform you
# will need to install the GNU coreutils with `brew install coreutils`
# and use `grealpath` instead.
REALPATH=realpath
if which grealpath 2>&1 >/dev/null; then
    REALPATH=grealpath
fi

INPATH=$($REALPATH $($REALPATH --relative-to=$CWD $1))

LIBDIR=$(mktemp -d)
cd $LIBDIR

python $AZURE_PATH/tools/psylib.py --extract $INPATH >/dev/null
echo *.OBJ

for OBJ in *.OBJ; do
    $PSYQ_OBJ_PARSER $OBJ -o $(basename $OBJ .OBJ).o >/dev/null
done

$AR rcs ${LIB}.a *.o
cp ${LIB}.a $CWD/

cd $HOME
rm -rf $LIBDIR
