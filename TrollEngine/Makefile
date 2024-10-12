# Add here if adding new directories
SRC_DIRS = AI Mario O2 Objects Trolls
SRC = $(shell find $(SRC_DIRS) -type f -iname '*.c' ! -iname '*.inc.c') $(shell find ./ -maxdepth 1 -type f -iname '*.c' ! -iname '*.inc.c')
OBJ = ${SRC:.c=.o}
ASM = $(shell find $(SRC_DIRS) -type f -iname '*.asm' | sed 's: :\\ :g') $(shell find ./ -maxdepth 1 -type f -iname '*.asm' | sed 's: :\\ :g')

PNG = $(shell find $(SRC_DIRS) -type f -iname '*.png') $(shell find ./ -maxdepth 1 -type f -iname '*.png')
PNG_INC_C = $(PNG:.png=.inc.c)

CC = tools/ido5.3_recomp/cc
ARMIPS = tools/armips
N64GRAPHICS = tools/n64graphics

CFLAGS = -c -G 0 -O2 -Wo,-loopunroll,0 -nostdinc -DTARGET_N64 -D_LANGUAGE_C -mips2 -IDecompHeaders/include -IDecompHeaders/src -IDecompHeaders/. -I. -IDecompHeaders/include/libc -IDecompHeaders/src/game -DVERSION_US=1 -DF3D_OLD=1 -non_shared -Wab,-r4300_mul -Xcpluscomm -Xfullwarn -signed -32


define print
	@printf "\033[0;32m$(1)\033[0m\n"
endef
define print2
	@printf "\033[0;32m$(1) \033[0;33m$(2)\033[0;32m -> \033[0;34m$(3)\033[0m\n"
endef

# Make sure tools are built
build_tools:
	$(call print,Building tools)
	cd "tools"; make -j$(nproc) all




%.o: %.c
	$(call print2,Compiling:,$<,$@)
	@$(CC) $(CFLAGS) -o '$@' '$<'

objs: $(PNG_INC_C) $(OBJ)


%.inc.c: %.png
	$(call print2,Converting:,$<,$@)
	@$(N64GRAPHICS) -s u8 -i '$@' -g '$<' -f rgba16

SILVED_BEE = ../Bee/b3313\ silved.z64
NEW_BEE = ../Bee/b3313\ new.z64

%.z64: build_tools objs $(ASM)
	$(call print,Applying ASM)
	$(ARMIPS) "apply deez balls in your mouth eediot.asm" -sym "sym.txt"
	$(call print,Calculating CRC Checksum)
	@chmod +x ./n64crc
	./n64crc $(NEW_BEE)
	@mv $(NEW_BEE) $(SILVED_BEE)

clean:
	@rm -f $(OBJ)

all: $(SILVED_BEE)

.DEFAULT_GOAL := all
.PHONY: all clean
