#ifndef AI_STATS_TRACKING_H
#define AI_STATS_TRACKING_H

// statistics of Mario's movement
// speed is set to max run speed by default, higher value -> player moving faster
extern f32 TRACKER_speed;
#define TRACKER_speed_LERP 0.000025f
// air time (0.0 grounded, 1.0 in air, 2.0 for wing cap)
extern f32 TRACKER_air_time;
#define TRACKER_air_time_LERP 0.000025f
// water (0.0 out of water, 1.0 in water, 2.0 swimming at top speed)
extern f32 TRACKER_water;
#define TRACKER_water_LERP 0.000025f

// more HP -> add stronger enemies/bosses
// less HP -> more healing needed
extern f32 TRACKER_hp;
#define TRACKER_hp_LERP 0.00005f
// increases for your hits, decreases for their hits. Should not directly be used, use the difficulty modifier (which uses this) instead!
extern f32 TRACKER_boss_performance;
// more deaths -> more lives needed to avoid game overs
extern f32 TRACKER_accum_deaths;
// drop one every ~22 minutes
#define TRACKER_accum_deaths_DECAY 0.000025f

// death reasons
// for each that is normalized to >1.0...
// if difficulty < 1.0, deaths like these should be avoided
// if difficulty > 1.0, traps for these should be placed
extern f32 TRACKER_accum_deaths_enemy_knockback;
extern f32 TRACKER_accum_deaths_skill_issue; // fall damage, drowning, quicksand, basically things that literally can't kill you unless you're bad lol
extern f32 TRACKER_accum_deaths_fire_lava;
extern f32 TRACKER_accum_deaths_death_barrier;
extern f32 TRACKER_accum_deaths_trolled; // traps as a result of personalization
// these are the values that should actually be used; they're normalized to an average of 1.0
extern f32 TRACKER_death_reasons[5];
// drop one every ~1.5 hours
#define TRACKER_accum_death_reasons_DECAY 0.00000625f

// social: a high value indicates the player likes to talk to NPCs (+0.1 if unlock, +0.2 if lore)
extern f32 TRACKER_accum_social;
// drop one every ~22 minutes
#define TRACKER_accum_social_DECAY 0.000025f

// nerd: a high value indicates the player likes to read (+0.25 per sign read)
extern f32 TRACKER_accum_nerd;
// drop one every ~22 minutes
#define TRACKER_accum_nerd_DECAY 0.000025f

// murder: a high value indicates the player likes to defeat enemies (+0.1 per enemy, +1.0 per boss)
extern f32 TRACKER_accum_murder;
// drop one every ~11 minutes
#define TRACKER_accum_murder_DECAY 0.00005f


// recent stars: a high value indicates breezing through the game
extern f32 TRACKER_accum_stars;
// drop one every ~11 minutes
#define TRACKER_accum_stars_DECAY 0.00005f
// the following are the stars categorized, used to target the player's preferences
// [gameplay]
extern f32 TRACKER_accum_stars_prefer_boss; //[X] auto assigned to stars dropped by bosses [0x01]
extern f32 TRACKER_accum_stars_prefer_cannon; //[X] requires manual assignment (0x01)
extern f32 TRACKER_accum_stars_prefer_collect; //[X] auto assigned to stars dropped by reds/smol stars, secrets [0x02]
extern f32 TRACKER_accum_stars_prefer_free; //[X] e.g. toad/mips stars, auto assigned, or can be manual if there's a random star with no challenge (0x02)
extern f32 TRACKER_accum_stars_prefer_murder; // the funny stars in b3313 where you commit mass genocide, some require manual assignment (0x04)
extern f32 TRACKER_accum_stars_prefer_platform; //[X] default in secret courses & stars at the top of the level, otherwise requires manual assignment (0x08)
extern f32 TRACKER_accum_stars_prefer_slide; //[X] default in terrain slide, can be manually assigned (0x10)
extern f32 TRACKER_accum_stars_prefer_timed; //[X] auto assigned to KtQ, slide stars, sm64.z64 star, etc [0x04]
// these are the values that should be used; they're normalized to an average of 1.0
extern f32 TRACKER_star_preferences_gameplay[8];
// [level type]
extern f32 TRACKER_accum_stars_prefer_buildings; // terrain stone+no bg (castles), terrain spooky
extern f32 TRACKER_accum_stars_prefer_overworld_courses; // terrain grass
extern f32 TRACKER_accum_stars_prefer_snow_courses; // terrain snow
extern f32 TRACKER_accum_stars_prefer_desert_courses; // terrain sand
extern f32 TRACKER_accum_stars_prefer_cave_courses; // funny personalization object set to cave
extern f32 TRACKER_accum_stars_prefer_water_courses; // terrain water
extern f32 TRACKER_accum_stars_prefer_fire_courses; // funny personalization object set to fire
extern f32 TRACKER_accum_stars_prefer_secret_courses; // terrain slide, terrain stone+bg (bowser courses)
// these are the values that should be used; they're normalized to an average of 1.0
extern f32 TRACKER_star_preferences_level_type[8];
// [caps] - stars that require a certain cap to collect (manual), or the stars within the cap courses (auto)
extern f32 TRACKER_accum_stars_prefer_wing_cap; // (0x20)
extern f32 TRACKER_accum_stars_prefer_metal_cap; // (0x40)
extern f32 TRACKER_accum_stars_prefer_vanish_cap; // (0x80)
// these are the values that should be used; they're normalized to an average of 1.0
extern f32 TRACKER_star_preferences_cap[3];
// drop one every ~44 minutes
#define TRACKER_accum_stars_prefer_DECAY 0.0000125f


// the following are "conclusions" based on the collected data
extern f32 TRACKER_prefer_collect;
extern f32 TRACKER_prefer_exploration;
extern f32 TRACKER_prefer_lore;
extern f32 TRACKER_prefer_murder;
extern f32 TRACKER_prefer_parkour;
extern f32 TRACKER_prefer_swimming;
// modifiers, 1.0 is the base value
extern f32 TRACKER_difficulty_modifier;
extern f32 TRACKER_level_scale_modifier_h;
extern f32 TRACKER_level_scale_modifier_v;
extern f32 TRACKER_difficulty_modifier_half;
extern f32 TRACKER_difficulty_modifier_sqrt_half;

extern u16 TRACKER_flags;
#define TRACKER_flag_bowser_spin_direction 0x0001


// Should be run once every frame
void TRACKER_record_mario_state(struct MarioState *m);

void TRACKER_on_npc_talk(register struct Object *o);

void TRACKER_inject_save();
void TRACKER_read_save();

#endif
