.orga 0x108a10
.area 0x40

; init level, sleep 2 frames, blackout false
.byte 0x1B, 0x04, 0x00, 0x00, 0x03, 0x04, 0x00, 0x02, 0x34, 0x04, 0x00, 0x00

; start level to RR
.byte 0x13, 4,   0x00, 0x0F

; execute levelscript jumptable
.byte 0x00, 0x18
.dh 0x15
.word 0x2abca0
.word 0x2ac6b0
.word 0x15000000
.word 0, 0

; jump ig
.byte 0x05, 0x08, 0, 0
.word 0x10000000

;define EXECUTE(seg, script, scriptEnd, entry) \
;    CMD_BBH(LEVEL_CMD_LOAD_AND_EXECUTE, 0x18, seg), \
;    CMD_PTR(script), \
;    CMD_PTR(scriptEnd), \
;    CMD_PTR(entry), \
;    CMD_PTR(NULL), \
;    CMD_PTR(NULL)
; EXECUTE(/*seg*/ SEGMENT_GLOBAL_LEVEL_SCRIPT, /*script*/ _scriptsSegmentRomStart, /*scriptEnd*/ _scriptsSegmentRomEnd, /*entry*/ level_main_scripts_entry),

.endarea
