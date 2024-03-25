/**
 * @file click_detection.h
 * @author Harun Špago (hspago@prosafety.ba)
 * @brief 
 * @version 0.1
 * @date 2024-03-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef CLICK_DETECTION_H
#define CLICK_DETECTION_H

#include <stdbool.h>

#define LOWER_NOISE 10
#define UPPER_NOISE 300
#define SHORT_LONG_CLICK_TRESHOLD 80
#define SHORT_CLICK_TIME_OUT 50


typedef enum{
    NOISE,
    CLICK,
    STAGE,
} ClickStates;

typedef enum{
    INITIAL,
    ONE_CLICK,
    DOUBLE_CLICK,
    TRIPLE_CLICK,
    LONG_CLICK
} ClickTypes;

typedef struct{
    unsigned int time_pressed;
    unsigned int time_released;
    unsigned char click_counter;
    ClickStates state;
} ClickData;

/**
 * @brief Function used to init the  click detection module.
 * 
 * @return ClickData 
 */
ClickData initClickDetection(void);
/**
 * @brief Function used to reset the time_pressed for the HIGH state of the gpio pin
 * 
 * @param click_data 
 */
void risingEdge(ClickData *click_data);
/**
 * @brief Function used to reset the time_relased for the LOW state of the gpio pin
 * 
 * @param click_data 
 */
void fallingEdge(ClickData *click_data);
/**
 * @brief Function used to increment the time pressed, aka the time the gpio pin is in the HIGH state
 * 
 * @param click_data 
 */
void incrementPressedTime(ClickData *click_data);
/**
 * @brief Function used to increment the time relased, aka the time the gpio pin is in LOW state
 * 
 * @param click_data 
 */
void incrementReleasedTime(ClickData *click_data);
/**
 * @brief Function used to procceses the current pin state and the previous informations and
 * change the informations of the click_data and return the detected click state
 * 
 * @param click_data 
 * @param curr_pin_state current state of the pin, bool
 * @return ClickTypes 
 */
ClickTypes proccesClickData(ClickData *click_data, bool curr_pin_state);


#endif // CLICK_DETECTION_H
