#ifndef AI_AUDIO_TROLLS_H
#define AI_AUDIO_TROLLS_H

void troll_sequence_player_process_sequence(register struct SequencePlayer *seqPlayer);
struct AudioBank *troll_load_banks_immediate(s32 seqId, u8 *outDefaultBank);

#endif
