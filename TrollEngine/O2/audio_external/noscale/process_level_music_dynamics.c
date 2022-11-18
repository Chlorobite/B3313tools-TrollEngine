#include "../audio_external_headers.h"

void process_level_music_dynamics(void) {
    register u32 conditionBits;
    register u32 tempBits;
    u8 musicDynIndex;
    register s32 condIndex;
    register s32 i;
    register s32 j;
    s16 conditionValues[8];
    u8 conditionTypes[8];
    register s16 dur1;
    register s16 dur2;
    register u16 bit;

    func_8031F96C(0);
    func_8031F96C(2);
    func_80320ED8();
    if (sMusicDynamicDelay != 0) {
        sMusicDynamicDelay--;
    } else {
        sBackgroundMusicForDynamics = sCurrentBackgroundMusicSeqId;
    }

    if (sBackgroundMusicForDynamics != sLevelDynamics[gCurrLevelNum][0]) {
        return;
    }

    conditionBits = sLevelDynamics[gCurrLevelNum][1] & 0xff00;
    musicDynIndex = (u8) sLevelDynamics[gCurrLevelNum][1] & 0xff;
    i = 2;
    while (conditionBits & 0xff00) {
        j = 0;
        condIndex = 0;
        bit = 0x8000;
        while (j < 8) {
            if (conditionBits & bit) {
                conditionValues[condIndex] = sLevelDynamics[gCurrLevelNum][i++];
                conditionTypes[condIndex] = j;
                condIndex++;
            }

            j++;
            bit = bit >> 1;
        }

        for (j = 0; j < condIndex; j++) {
            switch (conditionTypes[j]) {
                case MARIO_X_GE: {
                    if (((s16) gMarioStates[0].pos[0]) < conditionValues[j]) {
                        j = condIndex + 1;
                    }
                    break;
                }
                case MARIO_Y_GE: {
                    if (((s16) gMarioStates[0].pos[1]) < conditionValues[j]) {
                        j = condIndex + 1;
                    }
                    break;
                }
                case MARIO_Z_GE: {
                    if (((s16) gMarioStates[0].pos[2]) < conditionValues[j]) {
                        j = condIndex + 1;
                    }
                    break;
                }
                case MARIO_X_LT: {
                    if (((s16) gMarioStates[0].pos[0]) >= conditionValues[j]) {
                        j = condIndex + 1;
                    }
                    break;
                }
                case MARIO_Y_LT: {
                    if (((s16) gMarioStates[0].pos[1]) >= conditionValues[j]) {
                        j = condIndex + 1;
                    }
                    break;
                }
                case MARIO_Z_LT: {
                    if (((s16) gMarioStates[0].pos[2]) >= conditionValues[j]) {
                        j = condIndex + 1;
                    }
                    break;
                }
                case MARIO_IS_IN_AREA: {
                    if (gCurrAreaIndex != conditionValues[j]) {
                        j = condIndex + 1;
                    }
                    break;
                }
                case MARIO_IS_IN_ROOM: {
                    if (gMarioCurrentRoom != conditionValues[j]) {
                        j = condIndex + 1;
                    }
                    break;
                }
            }
        }

        if (j == condIndex) {
            // The area matches. Break out of the loop.
            tempBits = 0;
        } else {
            tempBits      = sLevelDynamics[gCurrLevelNum][i] & 0xff00;
            musicDynIndex = sLevelDynamics[gCurrLevelNum][i] & 0xff;
            i++;
        }

        conditionBits = tempBits;
    }

    if (sCurrentMusicDynamic != musicDynIndex) {
        tempBits = 1;
        if (sCurrentMusicDynamic == 0xff) {
            dur1 = 1;
            dur2 = 1;
        } else {
            dur1 = sMusicDynamics[musicDynIndex].dur1;
            dur2 = sMusicDynamics[musicDynIndex].dur2;
        }

        for (i = 0; i < CHANNELS_MAX; i++) {
            conditionBits = tempBits;
            tempBits = 0;
            if (sMusicDynamics[musicDynIndex].bits1 & conditionBits) {
                fade_channel_volume_scale(SEQ_PLAYER_LEVEL, i, sMusicDynamics[musicDynIndex].volScale1,
                                          dur1);
            }
            if (sMusicDynamics[musicDynIndex].bits2 & conditionBits) {
                fade_channel_volume_scale(SEQ_PLAYER_LEVEL, i, sMusicDynamics[musicDynIndex].volScale2,
                                          dur2);
            }
            tempBits = conditionBits << 1;
        }

        sCurrentMusicDynamic = musicDynIndex;
    }
}
