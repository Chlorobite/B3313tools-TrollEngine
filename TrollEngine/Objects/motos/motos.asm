; moto behavior
.headersize 0x80410000-0x3FF0000
.orga 0x3FF0000
.area 0x1000,0x33

// bhv script
.importobj "Objects/motos/bhvscripts.o"

// actual bhv
.importobj "Objects/motos/bhv.o"

.endarea


; normal moto model
.headersize 0x00410000-0x3FF0000
.orga 0x3FF1000
.area 0xC000,0x33

.importobj "Objects/motos/normal/models.o"

.endarea

; ice moto model
.headersize 0x00410000-0x3FE0000
.orga 0x3FE1000
.area 0xC000,0x33

.importobj "Objects/motos/ice/models.o"

.endarea
