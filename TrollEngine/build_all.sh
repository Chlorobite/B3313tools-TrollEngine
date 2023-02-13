sh build_o2.sh

sh build_ai.sh

sh build_mario.sh

sh build_trolls.sh

sh build_objects.sh

source ./_build_helpers.sh
build_c_files_in_dir ./

echo "Done, apply apply this.asm to a ROM with armips"
