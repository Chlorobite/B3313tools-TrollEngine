; Global star IDs: any star ID >=16 will directly correspond to a specific star ID in the save file,
; as opposed to referencing the current level's stars
; Some of the implementation (calls to blue_star_check) are bhv trolled (in Trolls/bhv),
; it's correct in both locations, but could possibly be moved here for clarity?

.headersize 0x8018F240
.orga 0x3D0DC0+0x780

.area 0x480
.importobj "Trolls/global_star_ids/blue_star_check.o"
.endarea
