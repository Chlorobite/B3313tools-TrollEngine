.definelabel rmseg5ram, 0x80720000

.definelabel _group1_SegmentStart, 0x05000000
.definelabel _group1_SegmentRomStart, 0x860EF0
.definelabel _group1_geoSegmentStart, 0x0c000000
.definelabel _group1_geoSegmentRomStart, 0x132850

.definelabel _group2_SegmentStart, 0x05000000
.definelabel _group2_SegmentRomStart, 0x87E250
.definelabel _group2_geoSegmentStart, 0x0c000000
.definelabel _group2_geoSegmentRomStart, 0x134a70

.definelabel _group3_SegmentStart, 0x05000000
.definelabel _group3_SegmentRomStart, 0x88C3D0
.definelabel _group3_geoSegmentStart, 0x0c000000
.definelabel _group3_geoSegmentRomStart, 0x13b5d0

.definelabel _group4_SegmentStart, 0x05000000
.definelabel _group4_SegmentRomStart, 0x8A5470
.definelabel _group4_geoSegmentStart, 0x0c000000
.definelabel _group4_geoSegmentRomStart, 0x145c10

.definelabel _group7_SegmentStart, 0x05000000
.definelabel _group7_SegmentRomStart, 0x8FB8B0
.definelabel _group7_geoSegmentStart, 0x0c000000
.definelabel _group7_geoSegmentRomStart, 0x1656e0

.definelabel _group11_SegmentStart, 0x05000000
.definelabel _group11_SegmentRomStart, 0x960230
.definelabel _group11_geoSegmentStart, 0x0c000000
.definelabel _group11_geoSegmentRomStart, 0x187fa0

.definelabel _group14_SegmentStart, 0x06000000
.definelabel _group14_SegmentRomStart, 0xA09960
.definelabel _group14_geoSegmentStart, 0x0d000000
.definelabel _group14_geoSegmentRomStart, 0x1d7c90

.definelabel _group15_SegmentStart, 0x06000000
.definelabel _group15_SegmentRomStart, 0xA36AF0
.definelabel _group15_geoSegmentStart, 0x0d000000
.definelabel _group15_geoSegmentRomStart, 0x1e4bf0

.definelabel _group17_SegmentStart, 0x06000000
.definelabel _group17_SegmentRomStart, 0xA647F0
.definelabel _group17_geoSegmentStart, 0x0d000000
.definelabel _group17_geoSegmentRomStart, 0x1f1b30

.definelabel _common0_SegmentStart, 0x08000000
.definelabel _common0_SegmentRomStart, 0xA81860
.definelabel _common0_geoSegmentStart, 0x0f000000
.definelabel _common0_geoSegmentRomStart, 0x2008d0

// amp, common0
// geolayout
.headersize _common0_geoSegmentStart-_common0_geoSegmentRomStart
.orga _common0_geoSegmentRomStart+(0x28-8) // alignment
.area 0x1a8-(0x28-8)
.importobj "Objects/junuary/amp/geo.o"
.orga _common0_geoSegmentRomStart+(0x28-8) // overwrite with old data
readword _common0_geoSegmentRomStart+(0x28-8)
readword _common0_geoSegmentRomStart+(0x28-4)
.endarea
// model
.headersize _common0_SegmentStart-_common0_SegmentRomStart
.orga _common0_SegmentRomStart+0x1318
.importobj "Objects/junuary/amp/model.o"

// bully, group2
// geolayout
.headersize _group2_geoSegmentStart-_group2_geoSegmentRomStart
.orga _group2_geoSegmentRomStart
.area 0x240
.importobj "Objects/junuary/bully/geo.o"
.endarea
// model
.headersize _group2_SegmentStart-_group2_SegmentRomStart
.orga _group2_SegmentRomStart
.area 0x4038
.importobj "Objects/junuary/bully/model.o"
.endarea

// chain chomp, group14
// geolayout
.headersize _group14_geoSegmentStart-_group14_geoSegmentRomStart
.orga _group14_geoSegmentRomStart+(0x5EC-0xC) // alignment
.area 0x680-(0x5EC-0xC)
.importobj "Objects/junuary/chain_chomp/geo.o"
.orga _group14_geoSegmentRomStart+(0x5EC-0xC) // overwrite with old data
readword _group14_geoSegmentRomStart+(0x5EC-0xC)
readword _group14_geoSegmentRomStart+(0x5EC-8)
readword _group14_geoSegmentRomStart+(0x5EC-4)
.endarea
// model
.headersize _group14_SegmentStart-_group14_SegmentRomStart
.orga _group14_SegmentRomStart+0x233D0 // only at chain_chomp_eye?
.area 0x25030-0x233D0
.importobj "Objects/junuary/chain_chomp/model.o"
.endarea

/* // chuckya was later reverted
// chuckya, common0
// segment 0x0F
.headersize _common0_geoSegmentStart-_common0_geoSegmentRomStart
.orga _common0_geoSegmentRomStart+(0x1D8-8) // alignment
.area 0x4cc-0x1d8
.importobj "Objects/junuary/chuckya/geo.o"
.orga _common0_geoSegmentRomStart+(0x1D8-8) // overwrite with old data
readword _common0_geoSegmentRomStart+(0x1D8-8)
readword _common0_geoSegmentRomStart+(0x1D8-4)
.endarea
// segment 0x08
.headersize _common0_SegmentStart-_common0_SegmentRomStart
.orga _common0_SegmentRomStart+0x66D0
.area 0xAC18-0x66D0
.importobj "Objects/junuary/chuckya/model.o"
.endarea
*/

// heavy hoe, group1
// geolayout
.headersize _group1_geoSegmentStart-_group1_geoSegmentRomStart
.orga _group1_geoSegmentRomStart+(0x28C-0xC) // alignment
.area (0x28C-0xC)
.importobj "Objects/junuary/heave_ho/geo.o"
.orga _group1_geoSegmentRomStart+(0x28C-0xC) // overwrite blank data with old data
readword _group1_geoSegmentRomStart+(0x28C-0xC)
readword _group1_geoSegmentRomStart+(0x28C-8)
readword _group1_geoSegmentRomStart+(0x28C-4)
.endarea
// model
.headersize _group1_SegmentStart-_group1_SegmentRomStart
.orga _group1_SegmentRomStart+0xE9C0
//.area 0x15360-0xE9C0 // eat into heave_ho_lights_unused5, we could probably make the area larger
.importobj "Objects/junuary/heave_ho/model.o"
.headersize rmseg5ram-_group1_SegmentRomStart // code cannot run from segmented address, thankfully rm just throws the groupA segment at the address it decides
.importobj "Objects/junuary/heave_ho/trollfunc.o"
//.endarea
// patch anim table address in bhv script
.orga rmseg13+0x1548+0x0C
.word heave_ho_seg5_anims_0501534C
// asm fix (rewinding anim doesn't exist, just use default anim)
.orga 0x6CE10
ADDIU A0, R0, 0x0000

// king bobomb, group3
// geolayout
.headersize _group3_geoSegmentStart-_group3_geoSegmentRomStart
.orga _group3_geoSegmentRomStart+0x0
.area 0x308
.importobj "Objects/junuary/king_bobomb/geo.o"
.endarea
// model
.headersize _group3_SegmentStart-_group3_SegmentRomStart
.orga _group3_SegmentRomStart+0x0
.area 0xFE68
.importobj "Objects/junuary/king_bobomb/model.o"
.endarea
// TODO: port the behavior code so the anims don't crash the game lol
// patch anim table address in bhv script
//.orga rmseg13+0x1f4+0xC ; bhvKingBobomb
//.word king_bobomb_seg5_anims_0500FE30

// koopa, group14
// geolayout
.headersize _group14_geoSegmentStart-_group14_geoSegmentRomStart
.orga _group14_geoSegmentRomStart+0xD0
.area (0x358+8)-0xd0
.importobj "Objects/junuary/koopa/geo.o"
.orga _group14_geoSegmentRomStart+0x358 // overwrite blank data with old data
readword _group14_geoSegmentRomStart+0x358
readword _group14_geoSegmentRomStart+0x358+4
.endarea
// model
.headersize _group14_SegmentStart-_group14_SegmentRomStart
.orga _group14_SegmentRomStart+0x25A0
.area 0xC6A0-0x25A0
.importobj "Objects/junuary/koopa/model.o"
.endarea

/* // mips was later reverted
// Million Instructions Per Second, group15
// geolayout
.headersize _group15_geoSegmentStart-_group15_geoSegmentRomStart
.orga _group15_geoSegmentRomStart+(0x448-8) // alignment
.area 0x5b0-(0x448-8)
.importobj "Objects/junuary/mips/geo.o"
.endarea
// model
.headersize _group15_SegmentStart-_group15_SegmentRomStart
.orga _group15_SegmentRomStart+0xFB80
.area 0x11FE0-0xFB80
.importobj "Objects/junuary/mips/model.o"
.endarea
*/

// penguin, group7
// geolayout
.headersize _group7_geoSegmentStart-_group7_geoSegmentRomStart
.orga _group7_geoSegmentRomStart+(0x104-4) // alignment
.area 0x21c-(0x104-4)
.importobj "Objects/junuary/penguin/geo.o"
.orga _group7_geoSegmentRomStart+0x100 // overwrite blank data with old data
readword _group7_geoSegmentRomStart+0x100
.endarea
// model
.headersize _group7_SegmentStart-_group7_SegmentRomStart
.orga _group7_SegmentRomStart+0x2D80
.area 0x7708-0x2D80
.importobj "Objects/junuary/penguin/model.o"
.endarea

// scuttlebug, group17
// geolayout
.headersize _group17_geoSegmentStart-_group17_geoSegmentRomStart
.orga _group17_geoSegmentRomStart+(0x394-4) // alignment
.area 0x6d0-(0x394-4)
.importobj "Objects/junuary/scuttlebug/geo.o"
.orga _group17_geoSegmentRomStart+(0x394-4) // overwrite blank data with old data
readword _group17_geoSegmentRomStart+(0x394-4)
.endarea
// model
.headersize _group17_SegmentStart-_group17_SegmentRomStart
.orga _group17_SegmentRomStart+0x100E0
.area 0x15070-0x100E0
.importobj "Objects/junuary/scuttlebug/model.o"
.endarea
// patch anim table address in bhv script
.orga rmseg13+0x2B5C+0x0C
.word scuttlebug_seg6_anims_06015064

// snowman, group7
// geolayout
.headersize _group7_geoSegmentStart-_group7_geoSegmentRomStart
.orga _group7_geoSegmentRomStart+(0x21C-0xC) // alignment
.area 0x370-(0x21C-0xC)
.importobj "Objects/junuary/snowman/geo.o"
.orga _group7_geoSegmentRomStart+(0x21C-0xC) // overwrite blank data with old data
readword _group7_geoSegmentRomStart+(0x21C-0xC)
readword _group7_geoSegmentRomStart+(0x21C-8)
readword _group7_geoSegmentRomStart+(0x21C-4)
.endarea
// model
.headersize _group7_SegmentStart-_group7_SegmentRomStart
.orga _group7_SegmentRomStart+0x8C10
.area 0xCD58-0x8C10
.importobj "Objects/junuary/snowman/model.o"
.endarea

// sushi & unagi, group4
// eat the entire space that sushi, unagi and whirlpool have combined
// unagi is moved to segment 1F, and we don't give a fuck about whirlpool
// geolayout
.headersize _group4_geoSegmentStart-_group4_geoSegmentRomStart
.orga _group4_geoSegmentRomStart+(0x68-8) // alignment
.area 0x10c-(0x68-8)
.importobj "Objects/junuary/sushi/geo.o"
.orga _group4_geoSegmentRomStart+(0x68-8) // overwrite blank data with old data
readword _group4_geoSegmentRomStart+(0x68-8)
readword _group4_geoSegmentRomStart+(0x68-4)
.endarea
// model
.headersize _group4_SegmentStart-_group4_SegmentRomStart
.orga _group4_SegmentRomStart+0x8ED0
.area 0x13D28-0x8ED0
.importobj "Objects/junuary/sushi/model.o"
.importobj "Objects/junuary/unagi/model.o"
.importobj "Objects/junuary/unagi/anims1.o"
.endarea
// patch anim table address in bhv script
.orga rmseg13+0x2338+0x0C
.word sushi_seg5_anims_0500AE54

// unagi, group4
// geolayout
.headersize _group4_geoSegmentStart-_group4_geoSegmentRomStart
.orga _group4_geoSegmentRomStart+(0x10C-0xC) // alignment
.area 0x280-(0x10C-0xC)
.importobj "Objects/junuary/unagi/geo.o"
.orga _group4_geoSegmentRomStart+(0x10C-0xC) // overwrite blank data with old data
readword _group4_geoSegmentRomStart+(0x10C-0xC)
readword _group4_geoSegmentRomStart+(0x10C-8)
readword _group4_geoSegmentRomStart+(0x10C-4)
.endarea
// model (imported above)
// anim separately in segment 1F
.headersize 0x00560000-0x3D0DC0
.orga 0x3D0DC0+0xDA00
.area 0x1E00
.importobj "Objects/junuary/unagi/anims2.o"
.endarea
// patch anim table address in bhv script
.orga rmseg13+0x4f40+0x0C
.word unagi_seg5_anims_05012824

// wigger, group11
// body
// geolayout
.headersize _group11_SegmentStart-_group11_SegmentRomStart
.orga _group11_SegmentRomStart+(0xC778-8) // alignment
.area 0xC878-(0xC778-8)
.importobj "Objects/junuary/wiggler/body_geo.o"
.orga _group11_SegmentRomStart+(0xC778-8) // overwrite blank data with old data
readword _group11_SegmentRomStart+(0xC778-8)
readword _group11_SegmentRomStart+(0xC778-4)
.endarea
// model
.headersize _group11_SegmentStart-_group11_SegmentRomStart
.orga _group11_SegmentRomStart+0x5A30
.area 0xC770-0x5A30
.importobj "Objects/junuary/wiggler/body_model.o"
.endarea
// patch anim table address in bhv script
.orga rmseg13+0x48e0+0x0C
.word wiggler_seg5_anims_0500C874
// head
// geolayout
.headersize _group11_geoSegmentStart-_group11_geoSegmentRomStart
.orga _group11_geoSegmentRomStart+0x30
.area 0x1bc-0x30
.importobj "Objects/junuary/wiggler/head_geo.o"
.endarea
// model
.headersize _group11_SegmentStart-_group11_SegmentRomStart
.orga _group11_SegmentRomStart+0xC878
.area 0xEC8C-0xC878
.importobj "Objects/junuary/wiggler/head_model.o"
.endarea
// patch anim table address in bhv script
.orga rmseg13+0x4898+0x10
.word wiggler_seg5_anims_0500EC8C

