#ifndef AI_POST_OBJECT_LOAD_PASS_H
#define AI_POST_OBJECT_LOAD_PASS_H

void area_geo_load_intercept(u8 areaId, u32 *areaGeoLayout);
void level_init_intercept();
void postObjectLoadPass();
void load_stuff_in_level_pool();

#endif
