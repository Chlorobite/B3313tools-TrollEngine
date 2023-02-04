.definelabel moto_bhv_start, 0x800000-0x20000
.definelabel moto_model_normal_start, moto_bhv_start+0x5000
; There is some inbetween data that, if replaced, seems to crash audio
; do not fuck with that, skip over it instead
; certified rom mangler moment
.definelabel moto_model_ice_start, 0x800000-0x8000

; moto behavior
.headersize 0x80410000-moto_bhv_start
.orga moto_bhv_start
.area 0xC00,0x33

// bhv script
.importobj "Objects/motos/bhvscripts.o"

// actual bhv
.importobj "Objects/motos/bhv.o"

.endarea

; moto animation
.headersize 0x00410000-moto_bhv_start
.orga moto_bhv_start+0xC00
.area 0x5000-0xC00,0x33

.importobj "Objects/motos/anim.o"

.endarea


; normal moto model
.headersize 0x00415000-moto_model_normal_start
.orga moto_model_normal_start
.area 0x8000,0x33

.importobj "Objects/motos/normal/models.o"

.endarea

; ice moto model
.headersize 0x00415000-moto_model_ice_start
.orga moto_model_ice_start
.area 0x8000,0x33

.importobj "Objects/motos/ice/models.o"

.endarea
