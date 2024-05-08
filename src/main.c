/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include <zephyr/drivers/i2c.h>
#include "../inc/i2c_lcd.h"

#define I2C_INST DT_NODELABEL(i2c1)

const struct device *const i2c = DEVICE_DT_GET(I2C_INST);

int main(){
	if (!device_is_ready(i2c)) {
		printf("i2c is not ready!\n");
	} else {
		printf("i2c is ready\n");
	}
	lcdInit(i2c);
	k_sleep(K_MSEC(1000));
	const char buf[] = "hello";
	lcdSendString(i2c, buf);
	while (1){
		printk("Hello world\n");
	}
	return 0;
}


