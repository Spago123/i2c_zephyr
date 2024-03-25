#include "../inc/sequence_impl.h"

void flashSequence(Sequence* sequence){
    if(sequence->timer >=             // SLOW                                               // FAST
    ((sequence->speed == SLOW) ? sequence->overflows[sequence->curr_transition] : sequence->overflows[sequence->curr_transition] / 2)){
        sequence->timer = 0;
        if(sequence->direction == REVERSE && sequence->curr_transition == 0) {
            sequence->curr_transition = sequence->num_of_transitions - 1;
        }else if(sequence->direction == FORWARD && sequence->curr_transition == sequence->num_of_transitions - 1){
            sequence->curr_transition = 0;
        }else {
            sequence->curr_transition = (sequence->direction == FORWARD) ? (sequence->curr_transition + 1) : (sequence->curr_transition - 1);
        }
    }
}

void changeSequenceDirection(Sequence *sequence){
    switch (sequence->direction) {
        case FORWARD:
            sequence->direction = REVERSE;
            break;
        case REVERSE:
            sequence->direction = FORWARD;
            break;
    }
}

void resetSequence(Sequence* sequence){
    sequence->timer = 0;
    sequence->speed = SLOW;
    sequence->direction = FORWARD;
    sequence->curr_transition = 0;
}

void changeSpeed(Sequence* sequence){
    if(sequence->speed == SLOW){
        sequence->speed = FAST;
    } else{
        sequence->speed = SLOW;
    }
}

void doTransition(FlashingSequences* flashing_sequences){
    if(flashing_sequences->curr_sequence == flashing_sequences->num_of_sequences - 1){
        flashing_sequences->curr_sequence = 0;
    } else{
        flashing_sequences->curr_sequence++;
    }
}

void sequenceFSM(FlashingSequences* flashing_sequences, ClickTypes click_types){
    switch (click_types){
        case INITIAL:{
            if (flashing_sequences->state == RESUME){
                flashSequence(&flashing_sequences->sequences[flashing_sequences->curr_sequence]);
            }
            break;
        }
        case ONE_CLICK:{
			if(flashing_sequences->state == RESUME){
				doTransition(flashing_sequences);
			    flashSequence(&flashing_sequences->sequences[flashing_sequences->curr_sequence]);
			}
            break;
        }
		case DOUBLE_CLICK:{
			if(flashing_sequences->state == RESUME){
				changeSpeed(&flashing_sequences->sequences[flashing_sequences->curr_sequence]);
				flashSequence(&flashing_sequences->sequences[flashing_sequences->curr_sequence]);
			}
            break;
		}
        case TRIPLE_CLICK:{
            if(flashing_sequences->state == RESUME){
                changeSequenceDirection(&flashing_sequences->sequences[flashing_sequences->curr_sequence]);
                flashSequence(&flashing_sequences->sequences[flashing_sequences->curr_sequence]);
            }
            break;
        }
		case LONG_CLICK:{
			if(flashing_sequences->state == RESUME){
				flashing_sequences->state = PAUSED;
			}else {
				flashing_sequences->state = RESUME;
			}
            break;
		}
    }
}
