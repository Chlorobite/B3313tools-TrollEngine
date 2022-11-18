.headersize 0x80245000

.org 0x80279c44
.area 0x200
.importobj "Trolls/global_star_ids/save_file_collect_star_or_key.o"
.endarea

.headersize 0x8018F240
.orga 0x3D0DC0+0x780

.area 0x480
.importobj "Trolls/global_star_ids/blue_star_check.o"
.endarea
