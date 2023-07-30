#include <PR/ultratypes.h>
#include <ultra64.h>

#include "audio/data.h"
#include "audio/effects.h"
#include "audio/external.h"
#include "audio/internal.h"
#include "audio/heap.h"
#include "audio/load.h"
#include "audio/seqplayer.h"
#include "audio/synthesis.h"


#include "AI/personalization_helpers.h"

#define PORTAMENTO_IS_SPECIAL(x) ((x).mode & 0x80)
#define PORTAMENTO_MODE(x) ((x).mode & ~0x80)
#define PORTAMENTO_MODE_1 1
#define PORTAMENTO_MODE_2 2
#define PORTAMENTO_MODE_3 3
#define PORTAMENTO_MODE_4 4
#define PORTAMENTO_MODE_5 5

void seq_channel_layer_process_script(struct SequenceChannelLayer *layer);
void sequence_channel_process_script(struct SequenceChannel *seqChannel);
u8 get_instrument(struct SequenceChannel *seqChannel, u8 instId, struct Instrument **instOut,
                  struct AdsrSettings *adsr);

extern int nowPlaying_seqId;
extern int nowPlaying_nlst;
extern int nowPlaying_tempo;
extern u8 lsd_textures;


void troll_sequence_player_process_sequence(register struct SequencePlayer *seqPlayer) {
    register u8 cmd;
    register u8 loBits;
    register u8 temp;
    register s32 value;
    register s32 i;
    register u16 u16v;
    register u8 *seqData;
    register struct M64ScriptState *state;

    if (seqPlayer->enabled == FALSE) {
        return;
    }

    if (seqPlayer->bankDmaInProgress == TRUE) {
        if (seqPlayer->bankDmaMesg == NULL) {
            return;
        }
        if (seqPlayer->bankDmaRemaining == 0) {
            seqPlayer->bankDmaInProgress = FALSE;
            patch_audio_bank(seqPlayer->loadingBank, gAlTbl->seqArray[seqPlayer->loadingBankId].offset,
                             seqPlayer->loadingBankNumInstruments, seqPlayer->loadingBankNumDrums);
            gCtlEntries[seqPlayer->loadingBankId].numInstruments = seqPlayer->loadingBankNumInstruments;
            gCtlEntries[seqPlayer->loadingBankId].numDrums = seqPlayer->loadingBankNumDrums;
            gCtlEntries[seqPlayer->loadingBankId].instruments = seqPlayer->loadingBank->instruments;
            gCtlEntries[seqPlayer->loadingBankId].drums = seqPlayer->loadingBank->drums;
            gBankLoadStatus[seqPlayer->loadingBankId] = SOUND_LOAD_STATUS_COMPLETE;
        } else {
            osCreateMesgQueue(&seqPlayer->bankDmaMesgQueue, &seqPlayer->bankDmaMesg, 1);
            seqPlayer->bankDmaMesg = NULL;
            audio_dma_partial_copy_async(&seqPlayer->bankDmaCurrDevAddr, &seqPlayer->bankDmaCurrMemAddr,
                                         &seqPlayer->bankDmaRemaining, &seqPlayer->bankDmaMesgQueue,
                                         &seqPlayer->bankDmaIoMesg);
        }
        return;
    }

    if (seqPlayer->seqDmaInProgress == TRUE) {
        if (seqPlayer->seqDmaMesg == NULL) {
            return;
        }
        seqPlayer->seqDmaInProgress = FALSE;
        gSeqLoadStatus[seqPlayer->seqId] = SOUND_LOAD_STATUS_COMPLETE;
    }

    // If discarded, bail out.
    if (IS_SEQ_LOAD_COMPLETE(seqPlayer->seqId) == FALSE
        || (
        IS_BANK_LOAD_COMPLETE(seqPlayer->defaultBank[0]) == FALSE)) {
        sequence_player_disable(seqPlayer);
        return;
    }

    // Remove possible SOUND_LOAD_STATUS_DISCARDABLE marks.
        gSeqLoadStatus[seqPlayer->seqId] = SOUND_LOAD_STATUS_COMPLETE;

        gBankLoadStatus[seqPlayer->defaultBank[0]] = SOUND_LOAD_STATUS_COMPLETE;

    if (seqPlayer->muted && (seqPlayer->muteBehavior & MUTE_BEHAVIOR_STOP_SCRIPT) != 0) {
        return;
    }

    // Check if we surpass the number of ticks needed for a tatum, else stop.
    {
        s32 trotempo = (seqPlayer != &gSequencePlayers[SEQ_PLAYER_LEVEL] || mus_tempooverride == 0 ? seqPlayer->tempo : (mus_tempooverride * TEMPO_SCALE));
        seqPlayer->tempoAcc += trotempo * (seqPlayer == &gSequencePlayers[SEQ_PLAYER_LEVEL] ?
            (nightMode ? 0.75f : 1.0f)
        : 1.0f);
        if (seqPlayer == &gSequencePlayers[SEQ_PLAYER_LEVEL])
            nowPlaying_tempo = trotempo / TEMPO_SCALE;
    }
    
    if (seqPlayer->tempoAcc < gTempoInternalToExternal) {
        return;
    }
    seqPlayer->tempoAcc -= (u16) gTempoInternalToExternal;

    state = &seqPlayer->scriptState;
    if (seqPlayer->delay > 1) {
        seqPlayer->delay--;
    } else {
        for (;;) {
            cmd = m64_read_u8(state);
            if (cmd == 0xff) { // seq_end
                if (state->depth == 0) {
                    sequence_player_disable(seqPlayer);
                    break;
                }
                state->depth--, state->pc = state->stack[state->depth];
            }

            if (cmd == 0xfd) { // seq_delay
                seqPlayer->delay = m64_read_compressed_u16(state);
                break;
            }

            if (cmd == 0xfe) { // seq_delay1
                seqPlayer->delay = 1;
                break;
            }

            if (cmd >= 0xc0) {
                switch (cmd) {
                    case 0xfc: // seq_call
                        u16v = m64_read_s16(state);
                        state->depth++, state->stack[state->depth - 1] = state->pc;
                        state->pc = seqPlayer->seqData + u16v;
                        break;

                    case 0xf8: // seq_loop; loop start, N iterations (or 256 if N = 0)
                        state->remLoopIters[state->depth] = m64_read_u8(state);
                        state->depth++, state->stack[state->depth - 1] = state->pc;
                        break;

                    case 0xf7: // seq_loopend
                        state->remLoopIters[state->depth - 1]--;
                        if (state->remLoopIters[state->depth - 1] != 0) {
                            state->pc = state->stack[state->depth - 1];
                        } else {
                            state->depth--;
                        }
                        break;

                    case 0xfb: // seq_jump
                    case 0xfa: // seq_beqz; jump if == 0
                    case 0xf9: // seq_bltz; jump if < 0
                    case 0xf5: // seq_bgez; jump if >= 0
                        u16v = m64_read_s16(state);
                        if (cmd == 0xfa && value != 0) {
                            break;
                        }
                        if (cmd == 0xf9 && value >= 0) {
                            break;
                        }
                        if (cmd == 0xf5 && value < 0) {
                            break;
                        }
                        state->pc = seqPlayer->seqData + u16v;
                        break;

                    case 0xf2: // seq_reservenotes
                        note_pool_clear(&seqPlayer->notePool);
                        note_pool_fill(&seqPlayer->notePool, m64_read_u8(state));
                        break;

                    case 0xf1: // seq_unreservenotes
                        note_pool_clear(&seqPlayer->notePool);
                        break;

                    case 0xdf: // seq_transpose; set transposition in semitones
                        seqPlayer->transposition = 0;
                        // fallthrough

                    case 0xde: // seq_transposerel; add transposition
                        seqPlayer->transposition += (s8) m64_read_u8(state);
                        break;

                    case 0xdd: // seq_settempo (bpm)
                    case 0xdc: // seq_addtempo (bpm)
                        temp = m64_read_u8(state);
                        if (cmd == 0xdd) {
                            seqPlayer->tempo = temp * TEMPO_SCALE;
                        } else {
                            seqPlayer->tempo += (s8) temp * TEMPO_SCALE;
                        }

                        if (seqPlayer->tempo > gTempoInternalToExternal) {
                            seqPlayer->tempo = gTempoInternalToExternal;
                        }

                        if ((s16) seqPlayer->tempo <= 0) {
                            seqPlayer->tempo = 1;
                        }
                        break;

                    case 0xdb: // seq_setvol
                        cmd = m64_read_u8(state);
                        switch (seqPlayer->state) {
                            case SEQUENCE_PLAYER_STATE_2:
                                if (seqPlayer->fadeRemainingFrames != 0) {
                                    f32 targetVolume = FLOAT_CAST(cmd) / 127.f;
                                    seqPlayer->fadeVelocity = (targetVolume - seqPlayer->fadeVolume)
                                                              / FLOAT_CAST(seqPlayer->fadeRemainingFrames);
                                    break;
                                }
                                // fallthrough
                            case SEQUENCE_PLAYER_STATE_0:
                                seqPlayer->fadeVolume = FLOAT_CAST(cmd) / 127.f;
                                break;
                            case SEQUENCE_PLAYER_STATE_FADE_OUT:
                            case SEQUENCE_PLAYER_STATE_4:
                                seqPlayer->volume = FLOAT_CAST(cmd) / 127.f;
                                break;
                        }
                        break;

                    case 0xda: // seq_changevol
                        temp = m64_read_u8(state);
                        seqPlayer->fadeVolume =
                            seqPlayer->fadeVolume + (f32)temp / 127.f;
                        break;

                    case 0xd7: // seq_initchannels
                        u16v = m64_read_s16(state);
                        sequence_player_init_channels(seqPlayer, u16v);
                        break;

                    case 0xd6: // seq_disablechannels
                        u16v = m64_read_s16(state);
                        sequence_player_disable_channels(seqPlayer, u16v);
                        break;

                    case 0xd5: // seq_setmutescale
                        temp = m64_read_u8(state);
                        seqPlayer->muteVolumeScale = (f32)temp / 127.f;
                        break;

                    case 0xd4: // seq_mute
                        seqPlayer->muted = TRUE;
                        break;

                    case 0xd3: // seq_setmutebhv
                        seqPlayer->muteBehavior = m64_read_u8(state);
                        break;

                    case 0xd2: // seq_setshortnotevelocitytable
                    case 0xd1: // seq_setshortnotedurationtable
                        u16v = m64_read_s16(state);
                        seqData = seqPlayer->seqData + u16v;
                        if (cmd == 0xd2) {
                            seqPlayer->shortNoteVelocityTable = seqData;
                        } else {
                            seqPlayer->shortNoteDurationTable = seqData;
                        }
                        break;

                    case 0xd0: // seq_setnoteallocationpolicy
                        seqPlayer->noteAllocPolicy = m64_read_u8(state);
                        break;

                    case 0xcc: // seq_setval
                        value = m64_read_u8(state);
                        break;

                    case 0xc9: // seq_bitand
                        value = m64_read_u8(state) & value;
                        break;

                    case 0xc8: // seq_subtract
                        value = value - m64_read_u8(state);
                        break;

                    default:
                        break;
                }
            } else {
                loBits = cmd & 0xf;
                switch (cmd & 0xf0) {
                    case 0x00: // seq_testchdisabled
                        if (IS_SEQUENCE_CHANNEL_VALID(seqPlayer->channels[loBits]) == TRUE) {
                            value = seqPlayer->channels[loBits]->finished;
                        }
                        break;
                    case 0x50: // seq_subvariation
                        value -= seqPlayer->seqVariation;
                        break;
                    case 0x70: // seq_setvariation
                        seqPlayer->seqVariation = value;
                        break;
                    case 0x80: // seq_getvariation
                        value = seqPlayer->seqVariation;
                        break;
                    case 0x90: // seq_startchannel
                        u16v = m64_read_s16(state);
                        sequence_channel_enable(seqPlayer, loBits, seqPlayer->seqData + u16v);
                        break;

                    default:
                        break;
                }
            }
        }
    }

    for (i = 0; i < CHANNELS_MAX; i++) {
        if (seqPlayer->channels[i] != &gSequenceChannelNone) {
            if (seqPlayer == &gSequencePlayers[SEQ_PLAYER_LEVEL]) {
                seqPlayer->channels[i]->transposition = mus_transposition;
                if (seqPlayer->channels[i]->freqScale == 1.0f)
                    seqPlayer->channels[i]->freqScale = mus_pitchmul;
            }
            sequence_channel_process_script(seqPlayer->channels[i]);
        }
    }
}

void troll_sequence_channel_process_script(struct SequenceChannel *seqChannel) {
    struct M64ScriptState *state;
    struct SequencePlayer *seqPlayer;
    u8 cmd;
    s8 temp;
    u8 loBits;
    u16 sp5A;
    s32 sp38;
    s8 value;
    s32 i;
    u8 *seqData;

    if (!seqChannel->enabled) {
        return;
    }

    if (seqChannel->stopScript) {
        for (i = 0; i < LAYERS_MAX; i++) {
            if (seqChannel->layers[i] != NULL) {
                seq_channel_layer_process_script(seqChannel->layers[i]);
            }
        }
        return;
    }

    seqPlayer = seqChannel->seqPlayer;
    if (seqPlayer->muted && (seqChannel->muteBehavior & MUTE_BEHAVIOR_STOP_SCRIPT) != 0) {
        return;
    }

    if (seqChannel->delay != 0) {
        seqChannel->delay--;
    }

    state = &seqChannel->scriptState;
    if (seqChannel->delay == 0) {
        for (;;) {
            cmd = m64_read_u8(state);
            if (cmd == 0xff) { // chan_end
                if (state->depth == 0) {
                    sequence_channel_disable(seqChannel);
                    break;
                }
                state->depth--, state->pc = state->stack[state->depth];
            }
            if (cmd == 0xfe) { // chan_delay1
                break;
            }
            if (cmd == 0xfd) { // chan_delay
                seqChannel->delay = m64_read_compressed_u16(state);
                break;
            }
            if (cmd == 0xf3) { // chan_hang
                seqChannel->stopScript = TRUE;
                break;
            }

            if (cmd > 0xc0)
            {
                switch (cmd) {
                    case 0xff: // chan_end
                        break;
                        
                    case 0xfc: // chan_call
                        sp5A = m64_read_s16(state);
                        state->depth++, state->stack[state->depth - 1] = state->pc;
                        state->pc = seqPlayer->seqData + sp5A;
                        break;

                    case 0xf8: // chan_loop; loop start, N iterations (or 256 if N = 0)
                        state->remLoopIters[state->depth] = m64_read_u8(state);
                        state->depth++, state->stack[state->depth - 1] = state->pc;
                        break;

                    case 0xf7: // chan_loopend
                        state->remLoopIters[state->depth - 1]--;
                        if (state->remLoopIters[state->depth - 1] != 0) {
                            state->pc = state->stack[state->depth - 1];
                        } else {
                            state->depth--;
                        }
                        break;

                    case 0xf6: // chan_break; break loop, if combined with jump
                        state->depth--;
                        break;

                    case 0xfb: // chan_jump
                    case 0xfa: // chan_beqz
                    case 0xf9: // chan_bltz
                    case 0xf5: // chan_bgez
                        sp5A = m64_read_s16(state);
                        if (cmd == 0xfa && value != 0)
                            break;
                        if (cmd == 0xf9 && value >= 0)
                            break;
                        if (cmd == 0xf5 && value < 0)
                            break;
                        state->pc = seqPlayer->seqData + sp5A;
                        break;
                        
                    case 0xf2: // chan_reservenotes
                        note_pool_clear(&seqChannel->notePool);
                        note_pool_fill(&seqChannel->notePool, m64_read_u8(state));
                        break;

                    case 0xf1: // chan_unreservenotes
                        note_pool_clear(&seqChannel->notePool);
                        break;

                    case 0xc2: // chan_setdyntable
                        sp5A = m64_read_s16(state);
                        seqChannel->dynTable = (void *) (seqPlayer->seqData + sp5A);
                        break;

                    case 0xc5: // chan_dynsetdyntable
                        if (value != -1) {
                            sp5A = (u16)((((*seqChannel->dynTable)[value])[0] << 8) + (((*seqChannel->dynTable)[value])[1]));
                            seqChannel->dynTable = (void *) (seqPlayer->seqData + sp5A);
                        }
                        break;

                    case 0xc1: // chan_setinstr ("set program"?)
                        set_instrument(seqChannel, m64_read_u8(state));
                        break;

                    case 0xc3: // chan_largenotesoff
                        seqChannel->largeNotes = FALSE;
                        break;

                    case 0xc4: // chan_largenoteson
                        seqChannel->largeNotes = TRUE;
                        break;

                    case 0xdf: // chan_setvol
                        sequence_channel_set_volume(seqChannel, m64_read_u8(state));
                        break;

                    case 0xe0: // chan_setvolscale
                        seqChannel->volumeScale = FLOAT_CAST(m64_read_u8(state)) / US_FLOAT(128.0);
                        break;

                    case 0xde: // chan_freqscale; pitch bend using raw frequency multiplier N/2^15 (N is u16)
                        sp5A = m64_read_s16(state);
                        seqChannel->freqScale = FLOAT_CAST(sp5A) / US_FLOAT(32768.0);
                        break;

                    case 0xd3: // chan_pitchbend; pitch bend by <= 1 octave in either direction (-127..127)
                        // (m64_read_u8(state) is really s8 here)
                        cmd = m64_read_u8(state) + 127;
                        seqChannel->freqScale = gPitchBendFrequencyScale[cmd];
                        break;

                    case 0xdd: // chan_setpan
                        seqChannel->pan = FLOAT_CAST(m64_read_u8(state)) / US_FLOAT(128.0);
                        break;

                    case 0xdc: // chan_setpanmix; set proportion of pan to come from channel (0..128)
                        seqChannel->panChannelWeight = FLOAT_CAST(m64_read_u8(state)) / US_FLOAT(128.0);
                        break;

                    case 0xdb: // chan_transpose; set transposition in semitones
                        temp = *state->pc++;
                        seqChannel->transposition = temp;
                        break;

                    case 0xda: // chan_setenvelope
                        sp5A = m64_read_s16(state);
                        seqChannel->adsr.envelope = (struct AdsrEnvelope *) (seqPlayer->seqData + sp5A);
                        break;

                    case 0xd9: // chan_setdecayrelease
                        seqChannel->adsr.releaseRate = m64_read_u8(state);
                        break;

                    case 0xd8: // chan_setvibratoextent
                        seqChannel->vibratoExtentTarget = m64_read_u8(state) * 8;
                        seqChannel->vibratoExtentStart = 0;
                        seqChannel->vibratoExtentChangeDelay = 0;
                        break;

                    case 0xd7: // chan_setvibratorate
                        seqChannel->vibratoRateStart = seqChannel->vibratoRateTarget =
                            m64_read_u8(state) * 32;
                        seqChannel->vibratoRateChangeDelay = 0;
                        break;

                    case 0xe2: // chan_setvibratoextentlinear
                        seqChannel->vibratoExtentStart = m64_read_u8(state) * 8;
                        seqChannel->vibratoExtentTarget = m64_read_u8(state) * 8;
                        seqChannel->vibratoExtentChangeDelay = m64_read_u8(state) * 16;
                        break;

                    case 0xe1: // chan_setvibratoratelinear
                        seqChannel->vibratoRateStart = m64_read_u8(state) * 32;
                        seqChannel->vibratoRateTarget = m64_read_u8(state) * 32;
                        seqChannel->vibratoRateChangeDelay = m64_read_u8(state) * 16;
                        break;

                    case 0xe3: // chan_setvibratodelay
                        seqChannel->vibratoDelay = m64_read_u8(state) * 16;
                        break;

                    case 0xd6: // chan_setupdatesperframe_unimplemented
                        cmd = m64_read_u8(state);
                        if (cmd == 0) {
                            cmd = gAudioUpdatesPerFrame;
                        }
                        seqChannel->updatesPerFrameUnused = cmd;
                        break;

                    case 0xd4: // chan_setreverb
                        seqChannel->reverbVol = m64_read_u8(state);
                        break;

                    case 0xc6: // chan_setbank; switch bank within set
                        cmd = m64_read_u8(state);
                        // Switch to the temp's (0-indexed) bank in this sequence's
                        // bank set. Note that in the binary format (not in the JSON!)
                        // the banks are listed backwards, so we counts from the back.
                        // (gAlBankSets[offset] is number of banks)
                        sp5A = ((u16 *) gAlBankSets)[seqPlayer->seqId];
                        loBits = *(sp5A + gAlBankSets);
                        cmd = gAlBankSets[sp5A + loBits - cmd];
                        if (get_bank_or_seq(&gBankLoadedPool, 2, cmd) != NULL)
                        {
                            seqChannel->bankId = cmd;
                        } else {
                            eu_stubbed_printf_1("SUB:ERR:BANK %d NOT CACHED.\n", cmd);
                        }
                        break;

                    case 0xc7: // chan_writeseq; write to sequence data (!)
                        {
                            u8 *seqData;
                            cmd = m64_read_u8(state);
                            sp5A = m64_read_s16(state);
                            seqData = seqPlayer->seqData + sp5A;
                            *seqData = (u8)value + cmd;
                        }
                        break;

                    case 0xc8: // chan_subtract
                    case 0xc9: // chan_bitand
                    case 0xcc: // chan_setval
                        temp = m64_read_u8(state);
                        if (cmd == 0xc8) {
                            value -= temp;
                        } else if (cmd == 0xcc) {
                            value = temp;
                        } else {
                            value &= temp;
                        }
                        break;

                    case 0xca: // chan_setmutebhv
                        seqChannel->muteBehavior = m64_read_u8(state);
                        break;

                    case 0xcb: // chan_readseq
                        sp38 = (u16)m64_read_s16(state) + value;
                        value = seqPlayer->seqData[sp38];
                        break;

                    case 0xd0: // chan_stereoheadseteffects
                        seqChannel->stereoHeadsetEffects = m64_read_u8(state);
                        break;

                    case 0xd1: // chan_setnoteallocationpolicy
                        seqChannel->noteAllocPolicy = m64_read_u8(state);
                        break;

                    case 0xd2: // chan_setsustain
                        seqChannel->adsr.sustain = m64_read_u8(state) << 8;
                        break;
                        
                    case 0xe4: // chan_dyncall
                        if (value != -1) {
                            seqData = (*seqChannel->dynTable)[value];
                            state->depth++, state->stack[state->depth - 1] = state->pc;
                            sp5A = ((seqData[0] << 8) + seqData[1]);
                            state->pc = seqPlayer->seqData + sp5A;
                        }
                        break;
                }
            } else {
                loBits = cmd & 0xf;

                switch (cmd & 0xf0) {
                    case 0x00: // chan_testlayerfinished
                        if (seqChannel->layers[loBits] != NULL) {
                            value = seqChannel->layers[loBits]->finished;
                        }
                        break;

                    // sh: 0x70
                    case 0x70: // chan_iowriteval; write data back to audio lib
                        seqChannel->soundScriptIO[loBits] = value;
                        break;

                    case 0x80: // chan_ioreadval; read data from audio lib
                        value = seqChannel->soundScriptIO[loBits];
                        if (loBits < 4) {
                            seqChannel->soundScriptIO[loBits] = -1;
                        }
                        break;

                    // sh: 0x50
                    case 0x50: // chan_ioreadvalsub; subtract with read data from audio lib
                        value -= seqChannel->soundScriptIO[loBits];
                        break;

                    case 0x90: // chan_setlayer
                        sp5A = m64_read_s16(state);
                        if (seq_channel_set_layer(seqChannel, loBits) == 0) {
                            seqChannel->layers[loBits]->scriptState.pc = seqPlayer->seqData + sp5A;
                        }
                        break;

                    case 0xa0: // chan_freelayer
                        seq_channel_layer_free(seqChannel, loBits);
                        break;

                    case 0xb0: // chan_dynsetlayer
                        if (value != -1 && seq_channel_set_layer(seqChannel, loBits) != -1) {
                            seqData = (*seqChannel->dynTable)[value];
                            sp5A = ((seqData[0] << 8) + seqData[1]);
                            seqChannel->layers[loBits]->scriptState.pc = seqPlayer->seqData + sp5A;
                        }
                        break;

                    case 0x60: // chan_setnotepriority (arg must be >= 2)
                        seqChannel->notePriority = loBits;
                        break;

                    case 0x10: // chan_startchannel
                        sp5A = m64_read_s16(state);
                        sequence_channel_enable(seqPlayer, loBits, seqPlayer->seqData + sp5A);
                        break;

                    case 0x20: // chan_disablechannel
                        sequence_channel_disable(seqPlayer->channels[loBits]);
                        break;

                    case 0x30: // chan_iowriteval2; write data back to audio lib for another channel
                        cmd = m64_read_u8(state);
                        seqPlayer->channels[loBits]->soundScriptIO[cmd] = value;
                        break;

                    case 0x40: // chan_ioreadval2; read data from audio lib from another channel
                        cmd = m64_read_u8(state);
                        value = seqPlayer->channels[loBits]->soundScriptIO[cmd];
                        break;
                }
            }
        }
    }

    for (i = 0; i < LAYERS_MAX; i++) {
        if (seqChannel->layers[i] != 0) {
            seq_channel_layer_process_script(seqChannel->layers[i]);
        }
    }
}

#define ALIGN16(val) (((val) + 0xF) & ~0xF)

struct SharedDma {
    /*0x0*/ u8 *buffer;       // target, points to pre-allocated buffer
    /*0x4*/ uintptr_t source; // device address
    /*0x8*/ u16 sizeUnused;   // set to bufSize, never read
    /*0xA*/ u16 bufSize;      // size of buffer
    /*0xC*/ u8 unused2;       // set to 0, never read
    /*0xD*/ u8 reuseIndex;    // position in sSampleDmaReuseQueue1/2, if ttl == 0
    /*0xE*/ u8 ttl;           // duration after which the DMA can be discarded
};                            // size = 0x10

extern u8 *gAlBankSets;


struct AudioBank *bank_load_immediate(s32 bankId, s32 arg1);

s32 lastSeqId = 0;
struct AudioBank *troll_load_banks_immediate(s32 seqId, u8 *outDefaultBank) {
    register void *ret;
    register u32 bankId;
    u16 offset;
    register s32 i;

    nowPlaying_seqId = seqId;

    offset = ((u16 *) gAlBankSets)[seqId];
    for (i = gAlBankSets[offset++]; i != 0; i--) {
        bankId = gAlBankSets[offset++];
        if (seqId >= 0x02) {
            if (mus_nlstday != 0)
                bankId = mus_nlstday;
            if (nightMode) {
                bankId = 0x14; // TODO: 0x1C for boss themes and "battlefields" (levels that have >10 goombas+bobombs)
                if (mus_nlstnight != 0)
                    bankId = mus_nlstnight;
            }
            if (lsd_textures) {
                // bad rng but idc
                do {
                    bankId = random_u16() & 0x3F;
                }
                while (bankId > 37);
            }
            
            nowPlaying_nlst = bankId;
        }

        if (IS_BANK_LOAD_COMPLETE(bankId)) {
            ret = get_bank_or_seq(&gBankLoadedPool, 2, bankId);
        } else {
            ret = NULL;
        }

        if (ret == NULL) {
            ret = bank_load_immediate(bankId, 2);
        }
    }
    *outDefaultBank = bankId;
    lastSeqId = seqId;
    return ret;
}

