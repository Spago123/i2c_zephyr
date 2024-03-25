#include "../inc/click_detection.h"


ClickData initClickDetection(void){
    return  (ClickData) {0, 0, 0, NOISE};
}

void risingEdge(ClickData *click_data){
    click_data->time_pressed = 0; // Reset time pressed to zero if it's a new button press
}

void fallingEdge(ClickData *click_data){
    click_data->time_released = 0;
}

void incrementPressedTime(ClickData *click_data){
    click_data->time_pressed++;
}
void incrementReleasedTime(ClickData *click_data){
    click_data->time_released++;
}

static bool checkShortClickConditions(unsigned int time_pressed){
    return time_pressed > LOWER_NOISE && time_pressed <= SHORT_LONG_CLICK_TRESHOLD;
}

static bool checkLongClickConditions(unsigned int time_pressed){
    return time_pressed > SHORT_LONG_CLICK_TRESHOLD && time_pressed <= UPPER_NOISE;
}

ClickTypes proccesClickData(ClickData *click_data, bool curr_pin_state){
    switch (click_data->state){
        case NOISE:{
            if (click_data->time_pressed > LOWER_NOISE && curr_pin_state){
                click_data->state = CLICK;
                return INITIAL;
            } else if(click_data->time_pressed <= LOWER_NOISE && !curr_pin_state){ // NOISE
                *click_data = initClickDetection();
                return INITIAL;
            }
            break;
        }
        case CLICK:{
            if(checkShortClickConditions(click_data->time_pressed) && !curr_pin_state){ // DETECTED A SHORT CLICK
                click_data->state = STAGE; 
                click_data->click_counter++;
                return INITIAL;
            } else if(checkLongClickConditions(click_data->time_pressed) && !curr_pin_state){ // LONG CLICK DETECTED
                *click_data = initClickDetection();
                return LONG_CLICK;
            } else if(click_data->time_pressed > UPPER_NOISE && !curr_pin_state){ // CONSIDER NOISE
                *click_data = initClickDetection();
                return INITIAL;
            }
            break;
        }
        case STAGE:{
            if (click_data->time_released > SHORT_CLICK_TIME_OUT && !curr_pin_state) {
                if(click_data->click_counter == 1) {
                    *click_data = initClickDetection();
                    return ONE_CLICK;
                } else if(click_data->click_counter == 2) {
                    *click_data = initClickDetection();
                    return DOUBLE_CLICK;
                } else if(click_data->click_counter == 3){
                    *click_data = initClickDetection();
                    return TRIPLE_CLICK;
                } else{
                    *click_data = initClickDetection(); 
                    return INITIAL;
                }
            } else if(click_data->time_released <= SHORT_CLICK_TIME_OUT && curr_pin_state){
                click_data->state = CLICK;
                return INITIAL;
            }
            break;
        }
    }
    return INITIAL;
}
