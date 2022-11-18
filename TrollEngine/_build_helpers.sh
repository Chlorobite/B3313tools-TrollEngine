build_c() {
	echo "Building $1 -> ${1::-2}.o"
	tools/ido5.3_recomp/cc -c -G 0 -O2 -Wo,-loopunroll,0 -nostdinc -DTARGET_N64 -D_LANGUAGE_C -mips2 -IDecompHeaders/include -IDecompHeaders/src -IDecompHeaders/. -I. -IDecompHeaders/include/libc -IDecompHeaders/src/game -DVERSION_US=1 -DF3D_OLD=1 -non_shared -Wab,-r4300_mul -Xcpluscomm -Xfullwarn -signed -32 -o ${1::-2}.o $1
}

build_c_files_in_dir() {
	DIR=$1
	# remove slash at end if exists
	if [[ $DIR == */ ]]; then
		DIR=${DIR::-1}
	fi
	
	for f in $DIR/*.c; do
		if [[ $f != *.inc.c ]]; then
			build_c $f
		fi
	done
}

convert_png_to_inc_c_in_dir() {
	DIR=$1
	# remove slash at end if exists
	if [[ $DIR == */ ]]; then
		DIR=${DIR::-1}
	fi
	
	for f in $DIR/*.png; do
		if [[ $f == *rgba16* ]]; then
			echo "Building $f -> ${f::-4}.inc.c"
			tools/n64graphics -s u8 -i "${f::-4}.inc.c" -g $f -f rgba16
		fi
	done
}
