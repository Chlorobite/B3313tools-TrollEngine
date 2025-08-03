// no way
.definelabel wf_seg7_lights_0700EE30, 0x0700EE30
.definelabel wf_seg7_vertex_0700EE48, 0x0700EE48
.definelabel wf_seg7_vertex_0700EF48, 0x0700EF48
.definelabel grassbeta_09007000, 0x09007000

.headersize 0x00560000-0x3D0DC0
.orga 0x3D0DC0+0x8000
.area 0x1000
.importobj "Objects/elevator_model/elevator_model.o"
.endarea

.orga 0x49E5BC
.word arrow_troll_platform
