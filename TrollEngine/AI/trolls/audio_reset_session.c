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

#define ALIGN16(val) (((val) + 0xF) & ~0xF)


struct PoolSplit {
    u32 wantSeq;
    u32 wantBank;
    u32 wantUnused;
    u32 wantCustom;
}; // size = 0x10

struct PoolSplit2 {
    u32 wantPersistent;
    u32 wantTemporary;
}; // size = 0x8


extern struct SoundAllocPool gAudioSessionPool;

extern struct PoolSplit sSessionPoolSplit;
extern struct PoolSplit2 sSeqAndBankPoolSplit;
extern struct PoolSplit sPersistentCommonPoolSplit;
extern struct PoolSplit sTemporaryCommonPoolSplit;

void audio_reset_session(struct AudioSessionSettings *preset) {
    s16 *mem;
#if defined(VERSION_JP) || defined(VERSION_US)
    s8 updatesPerFrame;
    s32 reverbWindowSize;
    s32 k;
#endif
    s32 i;
    s32 j;
    s32 persistentMem;
    s32 temporaryMem;
    s32 totalMem;
    s32 wantMisc;
#if defined(VERSION_JP) || defined(VERSION_US)
    s32 frames;
    s32 remainingDmas;
#else
    struct SynthesisReverb *reverb;
#endif

#if defined(VERSION_JP) || defined(VERSION_US)
    if (gAudioLoadLock != AUDIO_LOCK_UNINITIALIZED) {
        decrease_reverb_gain();
        for (i = 0; i < gMaxSimultaneousNotes; i++) {
            if (gNotes[i].enabled && gNotes[i].adsr.state != ADSR_STATE_DISABLED) {
                gNotes[i].adsr.fadeOutVel = 0x8000 / gAudioUpdatesPerFrame;
                gNotes[i].adsr.action |= ADSR_ACTION_RELEASE;
            }
        }

        // Wait for all notes to stop playing
        /*frames = 0;
        for (;;) {
            wait_for_audio_frames(1);
            frames++;
            if (frames > 4 * 60) {
                // Break after 4 seconds
                break;
            }

            for (i = 0; i < gMaxSimultaneousNotes; i++) {
                if (gNotes[i].enabled)
                    break;
            }

            if (i == gMaxSimultaneousNotes) {
                // All zero, break early
                break;
            }
        }*/

        // Wait for the reverb to finish as well
        decrease_reverb_gain();
        //wait_for_audio_frames(3);

        // The audio interface is double buffered; thus, we have to take the
        // load lock for 2 frames for the buffers to free up before we can
        // repurpose memory. Make that 3 frames, just in case.
        gAudioLoadLock = AUDIO_LOCK_LOADING;
        //wait_for_audio_frames(2);

        remainingDmas = gCurrAudioFrameDmaCount;
        while (remainingDmas > 0) {
            for (i = 0; i < gCurrAudioFrameDmaCount; i++) {
                if (osRecvMesg(&gCurrAudioFrameDmaQueue, NULL, OS_MESG_NOBLOCK) == 0)
                    remainingDmas--;
            }
        }
        gCurrAudioFrameDmaCount = 0;

        for (j = 0; j < NUMAIBUFFERS; j++) {
            for (k = 0; k < (s32) (AIBUFFER_LEN / sizeof(s16)); k++) {
                gAiBuffers[j][k] = 0;
            }
        }
    }
#endif

    gSampleDmaNumListItems = 0;
#if defined(VERSION_EU) || defined(VERSION_SH)
    gAudioBufferParameters.frequency = preset->frequency;
    gAudioBufferParameters.aiFrequency = osAiSetFrequency(gAudioBufferParameters.frequency);
    gAudioBufferParameters.samplesPerFrameTarget = ALIGN16(gAudioBufferParameters.frequency / gRefreshRate);
    gAudioBufferParameters.minAiBufferLength = gAudioBufferParameters.samplesPerFrameTarget - 0x10;
    gAudioBufferParameters.maxAiBufferLength = gAudioBufferParameters.samplesPerFrameTarget + 0x10;
#ifdef VERSION_SH
    gAudioBufferParameters.updatesPerFrame = (gAudioBufferParameters.samplesPerFrameTarget + 0x10) / 192 + 1;
    gAudioBufferParameters.samplesPerUpdate = (gAudioBufferParameters.samplesPerFrameTarget / gAudioBufferParameters.updatesPerFrame) & -8;
#else
    gAudioBufferParameters.updatesPerFrame = (gAudioBufferParameters.samplesPerFrameTarget + 0x10) / 160 + 1;
    gAudioBufferParameters.samplesPerUpdate = (gAudioBufferParameters.samplesPerFrameTarget / gAudioBufferParameters.updatesPerFrame) & 0xfff8;
#endif
    gAudioBufferParameters.samplesPerUpdateMax = gAudioBufferParameters.samplesPerUpdate + 8;
    gAudioBufferParameters.samplesPerUpdateMin = gAudioBufferParameters.samplesPerUpdate - 8;
    gAudioBufferParameters.resampleRate = 32000.0f / FLOAT_CAST(gAudioBufferParameters.frequency);
#ifdef VERSION_SH
    gAudioBufferParameters.unkUpdatesPerFrameScaled = (1.0f / 256.0f) / gAudioBufferParameters.updatesPerFrame;
#else
    gAudioBufferParameters.unkUpdatesPerFrameScaled = (3.0f / 1280.0f) / gAudioBufferParameters.updatesPerFrame;
#endif
    gAudioBufferParameters.updatesPerFrameInv = 1.0f / gAudioBufferParameters.updatesPerFrame;

    gMaxSimultaneousNotes = preset->maxSimultaneousNotes;
    gVolume = preset->volume;
    gTempoInternalToExternal = (u32) (gAudioBufferParameters.updatesPerFrame * 2880000.0f / gTatumsPerBeat / D_EU_802298D0);

    gAudioBufferParameters.presetUnk4 = preset->unk1;
    gAudioBufferParameters.samplesPerFrameTarget *= gAudioBufferParameters.presetUnk4;
    gAudioBufferParameters.maxAiBufferLength *= gAudioBufferParameters.presetUnk4;
    gAudioBufferParameters.minAiBufferLength *= gAudioBufferParameters.presetUnk4;
    gAudioBufferParameters.updatesPerFrame *= gAudioBufferParameters.presetUnk4;

#ifdef VERSION_SH
    if (gAudioBufferParameters.presetUnk4 >= 2) {
        gAudioBufferParameters.maxAiBufferLength -= 0x10;
    }
    gMaxAudioCmds = gMaxSimultaneousNotes * 0x14 * gAudioBufferParameters.updatesPerFrame + preset->numReverbs * 0x20 + 0x1E0;
#else
    gMaxAudioCmds = gMaxSimultaneousNotes * 0x10 * gAudioBufferParameters.updatesPerFrame + preset->numReverbs * 0x20 + 0x300;
#endif
#else
    reverbWindowSize = preset->reverbWindowSize;
    gAiFrequency = osAiSetFrequency(preset->frequency);
    gMaxSimultaneousNotes = preset->maxSimultaneousNotes;
    gSamplesPerFrameTarget = ALIGN16(gAiFrequency / 60);
    gReverbDownsampleRate = preset->reverbDownsampleRate;
    gVolume = preset->volume;
    gMinAiBufferLength = gSamplesPerFrameTarget - 0x10;
    updatesPerFrame = gSamplesPerFrameTarget / 160 + 1;
    gAudioUpdatesPerFrame = gSamplesPerFrameTarget / 160 + 1;

    // Compute conversion ratio from the internal unit tatums/tick to the
    // external beats/minute (JP) or tatums/minute (US). In practice this is
    // 300 on JP and 14360 on US.
#ifdef VERSION_JP
    gTempoInternalToExternal = updatesPerFrame * 3600 / gTatumsPerBeat;
#else
    gTempoInternalToExternal = (u32)(updatesPerFrame * 2880000.0f / gTatumsPerBeat / 16.713f);
#endif
    gMaxAudioCmds = gMaxSimultaneousNotes * 20 * updatesPerFrame + 320;
#endif

#if defined(VERSION_SH)
    persistentMem = DOUBLE_SIZE_ON_64_BIT(preset->persistentSeqMem + preset->persistentBankMem + preset->unk18 + preset->unkMem28 + 0x10);
    temporaryMem = DOUBLE_SIZE_ON_64_BIT(preset->temporarySeqMem + preset->temporaryBankMem + preset->unk24 + preset->unkMem2C + 0x10);
#elif defined(VERSION_EU)
    persistentMem = DOUBLE_SIZE_ON_64_BIT(preset->persistentSeqMem + preset->persistentBankMem);
    temporaryMem = DOUBLE_SIZE_ON_64_BIT(preset->temporarySeqMem + preset->temporaryBankMem);
#else
    persistentMem = DOUBLE_SIZE_ON_64_BIT(preset->persistentBankMem + preset->persistentSeqMem);
    temporaryMem = DOUBLE_SIZE_ON_64_BIT(preset->temporaryBankMem + preset->temporarySeqMem);
#endif
    totalMem = persistentMem + temporaryMem;
    wantMisc = gAudioSessionPool.size - totalMem - 0x100;
    sSessionPoolSplit.wantSeq = wantMisc;
    sSessionPoolSplit.wantCustom = totalMem;
    session_pools_init(&sSessionPoolSplit);
    sSeqAndBankPoolSplit.wantPersistent = persistentMem;
    sSeqAndBankPoolSplit.wantTemporary = temporaryMem;
    seq_and_bank_pool_init(&sSeqAndBankPoolSplit);
    sPersistentCommonPoolSplit.wantSeq = DOUBLE_SIZE_ON_64_BIT(preset->persistentSeqMem);
    sPersistentCommonPoolSplit.wantBank = DOUBLE_SIZE_ON_64_BIT(preset->persistentBankMem);
#ifdef VERSION_SH
    sPersistentCommonPoolSplit.wantUnused = preset->unk18;
#else
    sPersistentCommonPoolSplit.wantUnused = 0;
#endif
    persistent_pools_init(&sPersistentCommonPoolSplit);
    sTemporaryCommonPoolSplit.wantSeq = DOUBLE_SIZE_ON_64_BIT(preset->temporarySeqMem);
    sTemporaryCommonPoolSplit.wantBank = DOUBLE_SIZE_ON_64_BIT(preset->temporaryBankMem);
#ifdef VERSION_SH
    sTemporaryCommonPoolSplit.wantUnused = preset->unk24;
#else
    sTemporaryCommonPoolSplit.wantUnused = 0;
#endif
    temporary_pools_init(&sTemporaryCommonPoolSplit);
#ifdef VERSION_SH
    unk_pools_init(preset->unkMem28, preset->unkMem2C);
#endif
    reset_bank_and_seq_load_status();

#if defined(VERSION_JP) || defined(VERSION_US)
    for (j = 0; j < 2; j++) {
        gAudioCmdBuffers[j] = soundAlloc(&gNotesAndBuffersPool, gMaxAudioCmds * sizeof(u64));
    }
#endif

    gNotes = soundAlloc(&gNotesAndBuffersPool, gMaxSimultaneousNotes * sizeof(struct Note));
    note_init_all();
    init_note_free_list();

#if defined(VERSION_EU) || defined(VERSION_SH)
    gNoteSubsEu = soundAlloc(&gNotesAndBuffersPool, (gAudioBufferParameters.updatesPerFrame * gMaxSimultaneousNotes) * sizeof(struct NoteSubEu));

    for (j = 0; j != 2; j++) {
        gAudioCmdBuffers[j] = soundAlloc(&gNotesAndBuffersPool, gMaxAudioCmds * sizeof(u64));
    }

    for (j = 0; j < 4; j++) {
        gSynthesisReverbs[j].useReverb = 0;
    }
    gNumSynthesisReverbs = preset->numReverbs;
    for (j = 0; j < gNumSynthesisReverbs; j++) {
        reverb = &gSynthesisReverbs[j];
        reverbSettings = &preset->reverbSettings[j];
#ifdef VERSION_SH
        reverb->downsampleRate = reverbSettings->downsampleRate;
        reverb->windowSize = reverbSettings->windowSize * 64;
        reverb->windowSize /= reverb->downsampleRate;
#else
        reverb->windowSize = reverbSettings->windowSize * 64;
        reverb->downsampleRate = reverbSettings->downsampleRate;
#endif
        reverb->reverbGain = reverbSettings->gain;
#ifdef VERSION_SH
        reverb->panRight = reverbSettings->unk4;
        reverb->panLeft = reverbSettings->unk6;
        reverb->unk5 = reverbSettings->unk8;
        reverb->unk08 = reverbSettings->unkA;
#endif
        reverb->useReverb = 8;
        reverb->ringBuffer.left = soundAlloc(&gNotesAndBuffersPool, reverb->windowSize * 2);
        reverb->ringBuffer.right = soundAlloc(&gNotesAndBuffersPool, reverb->windowSize * 2);
        reverb->nextRingBufferPos = 0;
        reverb->unkC = 0;
        reverb->curFrame = 0;
        reverb->bufSizePerChannel = reverb->windowSize;
        reverb->framesLeftToIgnore = 2;
#ifdef VERSION_SH
        reverb->resampleFlags = A_INIT;
#endif
        if (reverb->downsampleRate != 1) {
#ifndef VERSION_SH
            reverb->resampleFlags = A_INIT;
#endif
            reverb->resampleRate = 0x8000 / reverb->downsampleRate;
            reverb->resampleStateLeft = soundAlloc(&gNotesAndBuffersPool, 16 * sizeof(s16));
            reverb->resampleStateRight = soundAlloc(&gNotesAndBuffersPool, 16 * sizeof(s16));
            reverb->unk24 = soundAlloc(&gNotesAndBuffersPool, 16 * sizeof(s16));
            reverb->unk28 = soundAlloc(&gNotesAndBuffersPool, 16 * sizeof(s16));
            for (i = 0; i < gAudioBufferParameters.updatesPerFrame; i++) {
                mem = soundAlloc(&gNotesAndBuffersPool, DEFAULT_LEN_2CH);
                reverb->items[0][i].toDownsampleLeft = mem;
                reverb->items[0][i].toDownsampleRight = mem + DEFAULT_LEN_1CH / sizeof(s16);
                mem = soundAlloc(&gNotesAndBuffersPool, DEFAULT_LEN_2CH);
                reverb->items[1][i].toDownsampleLeft = mem;
                reverb->items[1][i].toDownsampleRight = mem + DEFAULT_LEN_1CH / sizeof(s16);
            }
        }
#ifdef VERSION_SH
        if (reverbSettings->unkC != 0) {
            reverb->unk108 = sound_alloc_uninitialized(&gNotesAndBuffersPool, 16 * sizeof(s16));
            reverb->unk100 = sound_alloc_uninitialized(&gNotesAndBuffersPool, 8 * sizeof(s16));
            func_sh_802F0DE8(reverb->unk100, reverbSettings->unkC);
        } else {
            reverb->unk100 = NULL;
        }
        if (reverbSettings->unkE != 0) {
            reverb->unk10C = sound_alloc_uninitialized(&gNotesAndBuffersPool, 16 * sizeof(s16));
            reverb->unk104 = sound_alloc_uninitialized(&gNotesAndBuffersPool, 8 * sizeof(s16));
            func_sh_802F0DE8(reverb->unk104, reverbSettings->unkE);
        } else {
            reverb->unk104 = NULL;
        }
#endif
    }

#else
    if (reverbWindowSize == 0) {
        gSynthesisReverb.useReverb = 0;
    } else {
        gSynthesisReverb.useReverb = 8;
        gSynthesisReverb.ringBuffer.left = soundAlloc(&gNotesAndBuffersPool, reverbWindowSize * 2);
        gSynthesisReverb.ringBuffer.right = soundAlloc(&gNotesAndBuffersPool, reverbWindowSize * 2);
        gSynthesisReverb.nextRingBufferPos = 0;
        gSynthesisReverb.unkC = 0;
        gSynthesisReverb.curFrame = 0;
        gSynthesisReverb.bufSizePerChannel = reverbWindowSize;
        gSynthesisReverb.reverbGain = preset->reverbGain;
        gSynthesisReverb.framesLeftToIgnore = 2;
        if (gReverbDownsampleRate != 1) {
            gSynthesisReverb.resampleFlags = A_INIT;
            gSynthesisReverb.resampleRate = 0x8000 / gReverbDownsampleRate;
            gSynthesisReverb.resampleStateLeft = soundAlloc(&gNotesAndBuffersPool, 16 * sizeof(s16));
            gSynthesisReverb.resampleStateRight = soundAlloc(&gNotesAndBuffersPool, 16 * sizeof(s16));
            gSynthesisReverb.unk24 = soundAlloc(&gNotesAndBuffersPool, 16 * sizeof(s16));
            gSynthesisReverb.unk28 = soundAlloc(&gNotesAndBuffersPool, 16 * sizeof(s16));
            for (i = 0; i < gAudioUpdatesPerFrame; i++) {
                mem = soundAlloc(&gNotesAndBuffersPool, DEFAULT_LEN_2CH);
                gSynthesisReverb.items[0][i].toDownsampleLeft = mem;
                gSynthesisReverb.items[0][i].toDownsampleRight = mem + DEFAULT_LEN_1CH / sizeof(s16);
                mem = soundAlloc(&gNotesAndBuffersPool, DEFAULT_LEN_2CH);
                gSynthesisReverb.items[1][i].toDownsampleLeft = mem;
                gSynthesisReverb.items[1][i].toDownsampleRight = mem + DEFAULT_LEN_1CH / sizeof(s16);
            }
        }
    }
#endif

    init_sample_dma_buffers(gMaxSimultaneousNotes);

#if defined(VERSION_EU)
    build_vol_rampings_table(0, gAudioBufferParameters.samplesPerUpdate);
#endif

#ifdef VERSION_SH
    D_SH_8034F68C = 0;
    D_SH_803479B4 = 4096;
#endif

    osWritebackDCacheAll();

#if defined(VERSION_JP) || defined(VERSION_US)
    if (gAudioLoadLock != AUDIO_LOCK_UNINITIALIZED) {
        gAudioLoadLock = AUDIO_LOCK_NOT_LOADING;
    }
#endif
}
