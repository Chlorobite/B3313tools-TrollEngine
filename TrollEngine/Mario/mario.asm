.include "Mario/anims.asm"
.include "Mario/trolls.asm"

; mario squish threshold
.orga 0xF16E8
.word 0x41400000 ; 12 - original is 10.1f, and king whomp had a distance of 11
