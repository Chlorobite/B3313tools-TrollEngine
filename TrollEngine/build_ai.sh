. ./_build_helpers.sh

build_c_files_in_dir "AI/"
build_c_files_in_dir "AI/powermeter/**/"
convert_png_to_inc_c_in_dir "AI/more_models/"
build_c_files_in_dir "AI/more_models/"
