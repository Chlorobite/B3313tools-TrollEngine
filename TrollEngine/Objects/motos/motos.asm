; normal moto
; including this results in duplicate symbols :(
;// bhv script
;.headersize -0x3BE0000
;.orga 0x3FF0000
;.importobj "motos/bhvscripts.o"

;// actual bhv
;.headersize 0x7C420000
;.importobj "motos/bhv.o"

;// model
;.headersize -0x3BE0000
;.importobj "motos/normal/models.o"

; ice moto
.orga 0x3FE0000
.area 0x10000

// bhv script
.headersize -0x3BD0000
.importobj "Objects/motos/bhvscripts.o"

// actual bhv
.headersize 0x7C430000
.importobj "Objects/motos/bhv.o"

// model
.headersize -0x3BD0000
.importobj "Objects/motos/ice/models.o"

.endarea
