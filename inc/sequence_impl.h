/**
 * @file sequence_impl.h
 * @author Harun Špago (hspago@prosafety.ba)
 * @brief 
 * @version 0.1
 * @date 2024-03-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SEQUENCE_IMPL_H
#define SEQUENCE_IMPL_H

#include "click_detection.h"

#define TIMER_OVERFLOW 10 // in milli seconds
#define MAXIMUM 10

typedef enum{
    SLOW, 
    FAST
} Speed;

typedef enum{
    FORWARD, 
    REVERSE
} Direction;

typedef enum{
    RESUME,
    PAUSED
} SequenceState;

typedef struct {
    int timer;
    int curr_transition;
    int num_of_transitions;
    Speed speed;
    Direction direction;
	int overflows[MAXIMUM];
    int orange_pin_state[MAXIMUM];
    int red_pin_state[MAXIMUM];
    int green_pin_state[MAXIMUM];
    int blue_pin_state[MAXIMUM];
} Sequence;

typedef struct {
    Sequence sequences[MAXIMUM];
    int curr_sequence;
    int num_of_sequences;
    SequenceState state; 
} FlashingSequences;

void flashSequence(Sequence* sequence);

void sequenceFSM(FlashingSequences* flashing_sequences, ClickTypes click_types);

void resetSequence(Sequence* sequence);

void changeSpeed(Sequence* sequence);

void doTransition(FlashingSequences* flashing_sequences);

void changeSequenceDirection(Sequence *sequence);

#endif // SEQUENCE_IMPL_H
