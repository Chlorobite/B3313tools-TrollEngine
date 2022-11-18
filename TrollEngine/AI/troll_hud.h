#ifndef AI_TROLL_HUD_H
#define AI_TROLL_HUD_H

extern char hudType;

void set_hud_type(s32 bparam);

void troll_render_hud_mario_lives();
void troll_render_hud_coins();
void troll_render_hud_stars();
void troll_render_dl_power_meter();

#endif
