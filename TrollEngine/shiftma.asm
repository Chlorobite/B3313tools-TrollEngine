.n64
.open "../Bee/b3313 silved.z64", "../Bee/b3313 temp.z64", 0
.include "trollfinity.asm" ; the asm behind the trolling

; osPiStartDma
.headersize 0x80245000
.org 0x80324460
.area 0x80324570-0x80324460
.importobj "AI/trolls/osPiStartDma.o"
.endarea

; overcock in the rom header
.orga 4
.word 0x059682FF

.close
