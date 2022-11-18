#include <ultra64.h>
#include "sm64.h"
#include "game/level_update.h"
#include "level_commands.h"
#include "game/area.h"

#include "make_const_nonconst.h"

extern const GeoLayout silver_star_geo[];
extern const GeoLayout custom_door_1_geo[];
extern const GeoLayout custom_door_2_geo[];
extern const GeoLayout custom_door_3_geo[];
extern const GeoLayout custom_door_4_geo[];

const LevelScript more_models_levelscript[] = {
	LOAD_MODEL_FROM_GEO(0x97, silver_star_geo),
	LOAD_MODEL_FROM_GEO(0x98, custom_door_1_geo),
	LOAD_MODEL_FROM_GEO(0x99, custom_door_2_geo),
	LOAD_MODEL_FROM_GEO(0x9A, custom_door_3_geo),
	LOAD_MODEL_FROM_GEO(0x9B, custom_door_4_geo),
	RETURN(),
};
