/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../inc/click_detection.h"
#include "../inc/sequence_impl.h"
#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

#define STACKSIZE 1024
#define PRIORITY 7

static struct gpio_callback button_cb_data;

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led7)
#define USER_BTN_NODE DT_ALIAS(sw0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample do_cumentation for information on how to fix this.
 */
static const struct gpio_dt_spec green_led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec blue_led = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec orange_led = GPIO_DT_SPEC_GET(LED3_NODE, gpios);
static const struct gpio_dt_spec usr_btn = GPIO_DT_SPEC_GET(USER_BTN_NODE, gpios);

ClickData click_data;
ClickTypes click_type = INITIAL;
Sequence seq1, seq2, seq3;
FlashingSequences flashing_sequences;

struct k_timer my_timer;
volatile bool capture1;

void setLeds(Sequence* curr_sequence){
	gpio_pin_set_dt(&red_led, curr_sequence->red_pin_state[curr_sequence->curr_transition]);
	gpio_pin_set_dt(&orange_led, curr_sequence->orange_pin_state[curr_sequence->curr_transition]);
	gpio_pin_set_dt(&green_led, curr_sequence->green_pin_state[curr_sequence->curr_transition]);
	gpio_pin_set_dt(&blue_led, curr_sequence->blue_pin_state[curr_sequence->curr_transition]);
}

void do_(void){
	click_type = proccesClickData(&click_data, 
	((gpio_pin_get_dt(&usr_btn) == 1) ? true : false));
	sequenceFSM(&flashing_sequences, click_type);
	setLeds(&flashing_sequences.sequences[flashing_sequences.curr_sequence]);
	click_type = INITIAL;
}

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
	capture1 = true;
	if(gpio_pin_get_dt(&usr_btn)){
		risingEdge(&click_data);
	}else{
		fallingEdge(&click_data);
	}
}


void my_timer_handler(struct k_timer *dummy){
	flashing_sequences.sequences[flashing_sequences.curr_sequence].timer ++;
	capture1 = true;
	if(gpio_pin_get_dt(&usr_btn)){
		incrementPressedTime(&click_data);
	}else{
		incrementReleasedTime(&click_data);
	}
}
void app_1(void);

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);
K_THREAD_DEFINE(app_1_id, STACKSIZE, app_1, NULL, NULL, NULL, PRIORITY, NULL, NULL);

int main(){
	seq1 = (Sequence){
		0, 
		0, 
		5, 
		SLOW,
		FORWARD, 
		{50, 50, 50, 50, 50},
		{1, 0, 0, 1, 1},
		{1, 1, 0, 0, 1}, 
		{1, 1, 1, 0, 0}, 
		{0, 1, 1, 0, 1}		
	};
	seq2 = (Sequence){
		0, 
		0, 
		6, 
		SLOW,
		FORWARD, 
		{100, 50, 50, 100, 50, 50},
		{0, 1, 0, 0, 0, 1},
		{0, 0, 1, 0, 1, 0}, 
		{0, 0, 1, 0, 0, 1}, 
		{0, 1, 0, 0, 1, 0}		
	};
	seq3 = (Sequence){
		0, 
		0, 
		5, 
		SLOW,
		FORWARD, 
		{20, 40, 80, 160, 220},
		{0, 1, 1, 1, 1},
		{0, 0, 1, 1, 1}, 
		{0, 0, 0, 1, 1}, 
		{0, 0, 0, 0, 1}		
	};
	
	flashing_sequences = (FlashingSequences){
		{seq1, seq2, seq3}, 
		0, 
		3, 
		RESUME
	};
	click_data = initClickDetection();
	int ret, ret1, ret2, ret3, ret4;
	
	if(!device_is_ready(usr_btn.port)){
		printk("Error: button device is not ready\n");
		return 0;
	}


	if (!gpio_is_ready_dt(&green_led) || !gpio_is_ready_dt(&blue_led) ||
	 !gpio_is_ready_dt(&red_led) || !gpio_is_ready_dt(&orange_led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&green_led, GPIO_OUTPUT);
	ret1 = gpio_pin_configure_dt(&blue_led, GPIO_OUTPUT);
	ret2 = gpio_pin_configure_dt(&red_led, GPIO_OUTPUT);
	ret3 = gpio_pin_configure_dt(&orange_led, GPIO_OUTPUT);
	ret4 = gpio_pin_interrupt_configure_dt(&usr_btn, GPIO_INT_EDGE_BOTH);
	if (ret < 0 || ret1 < 0 || ret2 < 0 || ret3 < 0 || ret4 < 0) {
		return 0;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(usr_btn.pin));
	gpio_add_callback(usr_btn.port, &button_cb_data);
	capture1 = false;
	k_timer_start(&my_timer, K_MSEC(500), K_MSEC(10));
	return 0;
	
}

void app_1(){

	while (true) {
		if(capture1){
			capture1 = false;
			do_();
		}
	}
}




