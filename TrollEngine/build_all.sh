bash build_o2.sh

bash build_ai.sh

bash build_mario.sh

bash build_trolls.sh

bash build_objects.sh

. ./_build_helpers.sh
build_c_files_in_dir ./

echo "Done, apply apply this.asm to a ROM with armips"
