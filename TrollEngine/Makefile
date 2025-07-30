DEBUG ?= 1

SILVED_BEE = ../Bee/b3313\ silved.z64
SILVED_CONFIG = ../Bee/b3313\ silved.config
NEW_BEE = ../Bee/b3313\ new.z64
PAINTING_CFG = ../Bee/paintingcfg.txt
OPTIMIZED_BEE = ../Bee/b3313\ optimized.z64
OPTIMIZED_CONFIG = ../Bee/b3313\ optimized.config
SHIFTED_BEE = ../Bee/b3313\ shifted.z64

# Add here if adding new directories
SRC_DIRS = AI Mario O2 Objects Trolls
SRC = $(shell find $(SRC_DIRS) -type f -iname '*.c' ! -iname '*.inc.c') $(shell find ./ -maxdepth 1 -type f -iname '*.c' ! -iname '*.inc.c')
OBJ = ${SRC:.c=.o}
ASM = $(shell find $(SRC_DIRS) -type f -iname '*.asm' | sed 's: :\\ :g') $(shell find ./ -maxdepth 1 -type f -iname '*.asm' | sed 's: :\\ :g')

PNG = $(shell find $(SRC_DIRS) -type f -iname '*.png') $(shell find ./ -maxdepth 1 -type f -iname '*.png')
PNG_INC_C = $(PNG:.png=.inc.c)

CC = tools/ido-static-recomp/build/out/cc
ARMIPS = tools/armips
N64GRAPHICS = tools/n64graphics
MIO0 = tools/mio0
N64CKSUM = tools/n64cksum
PAINTING64 = tools/Painting64/Painting64
BEEIEOPTIMIZER = tools/BeeieOptimizer/BeeieOptimizer

ARMIPS_SILVED = $(ARMIPS) -strequ BEE $(SILVED_BEE)
ARMIPS_OPTIMIZED = $(ARMIPS) -strequ BEE $(OPTIMIZED_BEE)

CFLAGS = -c -G 0 -O2 -Wo,-loopunroll,0 -nostdinc -DTARGET_N64 -D_LANGUAGE_C -mips2 -IDecompHeaders/include -IDecompHeaders/src -IDecompHeaders/. -I. -IDecompHeaders/include/libc -IDecompHeaders/src/game -DVERSION_US=1 -DF3D_OLD=1 -non_shared -Wab,-r4300_mul -Xcpluscomm -Xfullwarn -signed -32

ifeq ($(DEBUG),1)
	CFLAGS += -DTROLLDEBUG=1
	ARMIPSDB = -definelabel TROLLDEBUG 0
endif


define print
	@printf "\033[0;32m$(1)\033[0m\n"
endef
define print2
	@printf "\033[0;32m$(1) \033[0;33m$(2)\033[0;32m -> \033[0;34m$(3)\033[0m\n"
endef


$(CC): $(shell find -iwholename '*tools/ido-static-recomp/*.c') $(shell find -iwholename '*tools/ido-static-recomp/*.cpp') $(shell find -iwholename '*tools/ido-static-recomp/*.h')
	cd tools; $(MAKE) -C ido-static-recomp setup; $(MAKE) -C ido-static-recomp
$(ARMIPS): tools/armips.cpp
	cd tools; c++ -pedantic -O2 -s -std=gnu++11 -fno-exceptions -fno-rtti -pipe armips.cpp -o armips -pthread -lm
$(TEXTCONV): tools/textconv.c tools/utf8.c tools/hashtable.c
	cd tools; cc -pedantic -O2 -s textconv.c utf8.c hashtable.c -o textconv -lm
$(N64GRAPHICS): tools/n64graphics.c tools/utils.c
	cd tools; cc -pedantic -O2 -s -DN64GRAPHICS_STANDALONE n64graphics.c utils.c -o n64graphics -lm
$(MIO0): tools/libmio0.c
	cd tools; cc -pedantic -O2 -s -DMIO0_STANDALONE libmio0.c -o mio0
$(N64CKSUM): tools/n64cksum.c tools/utils.c
	cd tools; cc -pedantic -O2 -s -DN64CKSUM_STANDALONE n64cksum.c utils.c -o n64cksum  -lm
# TODO: possibly include bee optimizer and painting64 as submodules
$(BEEIEOPTIMIZER): $(MIO0) $(N64GRAPHICS)

compile_commands.json: Makefile
	@echo '[{' > compile_commands.json
	@echo '  "directory": "'$$(pwd)'",' >> compile_commands.json
	@echo '  "command": "clang -std=gnu11 '$(DEFINES_INCLUDES)' -Wno-implicit-int -m32 -nostdinc -c '$$(pwd)'/ligma.c",' >> compile_commands.json
	@echo '  "file": "**/*.c"' >> compile_commands.json
	@echo '}]' >> compile_commands.json

# compile_commands.json is very much crucial to the mario
TOOLS = $(CC) $(ARMIPS) $(TEXTCONV) $(N64GRAPHICS) $(MIO0) $(N64CKSUM) compile_commands.json

tools: $(TOOLS)
	$(call print,Building tools...)




%.o: %.c $(CC)
	$(call print2,Compiling:,$<,$@)
	@$(CC) $(CFLAGS) -o '$@' '$<'

objs: $(PNG_INC_C) $(OBJ)


%.inc.c: %.png $(N64GRAPHICS)
	$(call print2,Converting:,$<,$@)
	@$(N64GRAPHICS) -s u8 -i '$@' -g '$<' -f rgba16

$(SILVED_BEE): objs $(ASM) $(ARMIPS) $(N64CKSUM) compile_commands.json
	$(call print,(Apply) Applying ASM)
	$(ARMIPS_SILVED) "apply deez balls in your mouth eediot.asm" -sym "sym.txt" $(ARMIPSDB)
	@mv $(NEW_BEE) $(SILVED_BEE)
ifeq ($(shell test -e $(PAINTING64) && test -e $(PAINTING_CFG) && echo -n pain),pain)
	$(call print,(Apply) Applying Painting64)
	@chmod +x $(PAINTING64)
	@cd ../Bee && ../TrollEngine/$(PAINTING64) $(SILVED_BEE) --automatic
else
	$(call print,(Apply) Please install Painting64 into $(PAINTING64) & set up paintingcfg.txt to apply paintings)
endif
	$(call print,(Apply) Calculating CRC Checksum)
	@$(N64CKSUM) $(SILVED_BEE)

apply: $(SILVED_BEE)


$(OPTIMIZED_BEE): $(SILVED_BEE) $(ARMIPS) $(N64CKSUM) $(BEEIEOPTIMIZER)
ifeq ($(shell test -e $(PAINTING64) && test -e $(PAINTING_CFG) && test -e $(BEEIEOPTIMIZER) && echo -n pain),pain)
	@cp $(SILVED_BEE) $(OPTIMIZED_BEE)
	@cp $(SILVED_CONFIG) $(OPTIMIZED_CONFIG)
	@chmod +x $(BEEIEOPTIMIZER)
	$(call print,(Optimize) Optimizing the bee)
	$(BEEIEOPTIMIZER) $(OPTIMIZED_BEE) run $(PAINTING64) $(PAINTING_CFG)
	$(call print,(Optimize) Applying ASM again (unmangle the rom))
	$(ARMIPS_OPTIMIZED) "apply deez balls in your mouth eediot.asm" -sym "sym.txt" $(ARMIPSDB)
	$(call print,(Optimize) Calculating CRC Checksum)
	@$(N64CKSUM) $(NEW_BEE)
	@mv $(NEW_BEE) $(OPTIMIZED_BEE)
else
	$(call print,(Optimize) Please install Painting64 into $(PAINTING64) & BeeieOptimizer into $(BEEIEOPTIMIZER) & set up paintingcfg.txt to create optimized bee)
endif

$(SHIFTED_BEE): $(OPTIMIZED_BEE) $(ARMIPS) $(N64CKSUM)
ifeq ($(shell test -e $(PAINTING64) && test -e $(PAINTING_CFG) && test -e $(BEEIEOPTIMIZER) && echo -n pain),pain)
	$(call print,(Shift) Applying ASM)
	$(ARMIPS_OPTIMIZED) "shiftma.asm"
	$(call print,(Shift) Trimming to 64 MB)
	@{ head -c 1004928 "../Bee/b3313 temp.z64"; printf 'YAHAAAAAAAAAAAAA'; tail -c +1004929 "../Bee/b3313 temp.z64"; } | head -c 67108864 > $(SHIFTED_BEE)
	$(call print,(Shift) Calculating CRC Checksum)
	@$(N64CKSUM) $(SHIFTED_BEE)
	$(call print,(Shift) Removing temporary files)
	@rm -f "../Bee/b3313 temp.z64"
endif

release: $(SHIFTED_BEE)


clean:
	@rm -f $(PNG_INC_C) $(OBJ)

cleantools: clean
	@cd tools; $(MAKE) -C ido-static-recomp clean > /dev/null
	@rm -rf $(TOOLS)

.DEFAULT_GOAL := apply
.PHONY: tools clean cleantools apply release
