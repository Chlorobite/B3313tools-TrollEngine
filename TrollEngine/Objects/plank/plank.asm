.headersize 0x0018F240
.orga 0x3D0DC0+0x8C00
.area 0x1800
.importobj "Objects/plank/plank_geo.o"
.importobj "Objects/plank/plank_model.o"
.endarea

; probably not a good idea to shift the geo around, you would have to fix level scripts like this:
; .orga 0x16D08E8
; .word wf_geo_000A58
