#!/bin/bash
rm -f "../Bee/b3313 temp.z64"
rm -f "../Bee/b3313 copped.z64"

./tools/armips "shiftma.asm"

if [ ! -f "../Bee/b3313 temp.z64" ]; then
    echo "No new romge found, do you have armips installed?"
    exit
fi

{ head -c 1004928 "../Bee/b3313 temp.z64"; printf 'YAHAAAAAAAAAAAAA'; tail -c +1004929 "../Bee/b3313 temp.z64"; } > "../Bee/b3313 shiftma.z64"
{ head -c 50331648 "../Bee/b3313 shiftma.z64"; } > "../Bee/b3313 copped.z64"

rm -f "../Bee/b3313 temp.z64"
rm -f "../Bee/b3313 shiftma.z64"
chmod +x ./n64crc
./n64crc "../Bee/b3313 copped.z64"
