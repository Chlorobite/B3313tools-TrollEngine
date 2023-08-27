#ifndef AI_PERSONALIZATION_HELPERS_H
#define AI_PERSONALIZATION_HELPERS_H

#include "engine/graph_node.h"
#include "game/area.h"

extern char levelType;
extern char goombaType, bobombType, ampType;
extern f32 levelScaleH, levelScaleV;
extern u8 renderingLevel;
extern u8 rtcHour, rtcMinute, rtcSecond;
extern u8 nightTime;
extern u8 nightTimeButReal;
extern u8 nightMode;
extern u8 nightModeSetting;
extern u8 sunsetMode;
extern u8 skyboxLoaded;
extern u8 spawningLootCoins;
extern u8 spawningBossStar;
extern u16 personalizationRandSeed;
extern u32 personalization_beeparams;
extern s32 isInstantWarping;
extern s32 loadedObjectCount;

extern u8 mus_tempooverride;
extern f32 mus_pitchmul;
extern s16 mus_transposition;
extern u8 mus_nlstday;
extern u8 mus_nlstnight;

extern u8 personalizationFlags;
#define PERSONALIZATION_FLAG_DISABLE_DISPLAY_LIST  (personalizationFlags&0x01)
#define PERSONALIZATION_FLAG_DISABLE_OBJECTS       (personalizationFlags&0x02)
#define PERSONALIZATION_FLAG_DISABLE_RETEXTURE     (personalizationFlags&0x04)
#define PERSONALIZATION_FLAG_DISABLE_LEVEL_SCALE   (personalizationFlags&0x08)
#define PERSONALIZATION_FLAG_ENABLE_TOXIC_GAS      (personalizationFlags&0x10)
#define PERSONALIZATION_FLAG_LOAD_RED_STAR         (personalizationFlags&0x20)
#define PERSONALIZATION_FLAG_HUE_SHIFT_STAR        (personalizationFlags&0x40)
#define PERSONALIZATION_FLAG_TROLL_FAST_TRAVEL     (personalizationFlags&0x80)

void hueRotateRGBA5551(u16 *ptr, int size, int hue);
void rgbMultiplyRGBA16(u16 *ptr, int size, float _r, float _g, float _b);
void texCopyRGBA16(u16 *ptrFrom, u16 *ptrTo, int size);
void darkenRGBA16(u16 *ptr, int size);
void darkenRGBA32(u32 *ptr, int size);
void rotateRGBA16(u16 *ptr, int size);
void rotateRGBA32(u32 *ptr, int size);
void invertRGBA16(u16 *ptr, int size, u8 lsd_texture_type);
void invertRGBA32(u32 *ptr, int size, u8 lsd_texture_type);
void normalizeRGBA16(u16 *ptr, int size);
void normalizeRGBA32(u32 *ptr, int size);
void unfuckSegment(u8 segment);
void updateRTC();

extern u8 overrideBgTerrain;
extern u8 overrideBgId;

void interceptSegLoad(u8 segId, u8 **segStart, u8 **segEnd);
s32 coin_troll_hitbox(struct Object *o);
void personalize_coin(struct Object *o, u8 type);
void personalized_interact_coin(struct MarioState *m, struct Object *o);
u32 lava_goomba_funny_burn_mario(register struct Object *_o, register struct MarioState *m);
extern u8 sGoombaImmuneAttackHandlers[6];
void obj_scale_personalized(struct Object *obj);

s32 get_model_loaded(u8 model);
void set_model_loaded(u8 model, s32 isLoaded);
void clear_model_loaded();

void on_collected_star(struct Object *starObj);
void AI_star_set_platform_bparam_if_above_average_y(struct Object *starObj);
void AI_star_set_bparams(struct Object *starObj);
f32 AI_star_get_preference_by_bparams(struct Object *starObj);

void scaled_geo_append_display_list(void *displayList, s32 layer);
void troll_load_environmental_regions(register s16 **data);
void vec3f_scaled_set(register Vec3f dest, register f32 x, register f32 y, register f32 z);
void scale_instant_warp(register struct InstantWarp *warp);

struct GraphNodeBackground *troll_init_graph_node_background(struct AllocOnlyPool *pool,
                                                       struct GraphNodeBackground *graphNode,
                                                       u16 background, GraphNodeFunc backgroundFunc);
void *troll_dynamic_dma_read_and_set_segment(u8 *srcStart, u8 *srcEnd, u32 side, register s32 segment);

void on_death_barrier_warp();
void spawn_boss_star(register void *weirdptr);

u32 troll_bowser_actions_bitdw_not_reacting(register struct Object *_o, register f32 rand, register f32 diff_div_10);
u32 troll_bowser_actions_bitfs_not_reacting(register struct Object *_o, register f32 rand, register f32 diff_div_10);
u32 troll_bowser_actions_bits_angle_mario(register struct Object *_o, register f32 rand, register f32 diff_div_10);
u32 troll_bowser_actions_bits_reacting(register struct Object *_o, register f32 rand, register f32 diff_div_10);
s32 troll_bowser_check_hit_mine();
s32 troll_bowser_nyoom(register struct Object *_o);
void troll_mark_goomba_as_dead();
void troll_goomba_wander(register struct Object *_o);
void troll_yellow_switch_or_set_flags(s32 __oBehParams2ndByte);
void fuck_textures();
s32 troll_perform_ground_step(register struct MarioState *m);
void troll_change_area(register s32 index);
void troll_toad_message_talking();
s32 troll_cur_obj_update_dialog(s32 actionArg, s32 dialogFlags, s32 dialogID);
struct Object *troll_spawn_star(struct Object *sp30, f32 sp34, f32 sp38, f32 sp3C);
void troll_spawn_bowser_star();

s32 can_pass_through_walls();

struct AllocOnlyPool *troll_render_pool_init();

s32 set_mario_animation_then_check_for_climbable_wall(struct MarioState *m, s32 animation);
s32 act_electric_idle(struct MarioState *m);

struct Object *troll_spawn_star_bypass_magnets(struct Object *sp30, f32 sp34, f32 sp38, f32 sp3C);

s32 is_object_star_spawner(struct Object *obj);

s32 is_pointer_valid(void *ptr);

#endif
