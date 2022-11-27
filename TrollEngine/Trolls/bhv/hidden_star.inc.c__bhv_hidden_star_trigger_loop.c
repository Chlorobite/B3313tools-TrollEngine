#include "bhv_headers.h"


void spawn_red_coin_cutscene_star(f32 x, f32 y, f32 z);
void bhv_hidden_star_trigger_loop(void) {
    register struct Object *_o = o;
    if (_o->oBehParams2ndByte != 0) {
        _o->hitboxRadius = _o->oBehParams2ndByte * 10;
        _o->hitboxHeight = _o->oBehParams2ndByte * 10;
    }

    if (obj_check_if_collided_with_object(_o, gMarioObject) == TRUE) {
        register struct Object *hiddenStar = cur_obj_nearest_object_with_behavior(bhvHiddenStar);

        if (hiddenStar != NULL) {
            if (++hiddenStar->oHiddenStarTriggerCounter != 5) {
                spawn_orange_number(hiddenStar->oHiddenStarTriggerCounter, 0, 0, 0);
            }

            play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundSource);
        }

        _o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}

