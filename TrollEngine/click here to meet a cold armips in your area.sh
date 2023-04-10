armips "apply deez balls in your mouth eediot.asm" -sym "sym.txt"

if [ ! -f "../Bee/b3313 new.z64" ]; then
    echo "No new romge found, do you have armips installed?"
    exit
fi

chmod +x ./n64crc
./n64crc "../Bee/b3313 new.z64"
cp "../Bee/b3313 silved.config" "../Bee/b3313 new.config"
rm "../Bee/b3313 silved.z64"
rm "../Bee/b3313 silved.config"
mv "../Bee/b3313 new.z64" "../Bee/b3313 silved.z64"
mv "../Bee/b3313 new.config" "../Bee/b3313 silved.config"
