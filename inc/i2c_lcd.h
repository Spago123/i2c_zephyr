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
#ifndef I2C_LCD_H
#define I2C_LCD_H

#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#define SLAVE_ADDRESS_LCD 0x4E

/**
 * @brief Send a command to the LCD.
 *
 * @param i2c_dev Pointer to the I2C device.
 * @param cmd Command byte to send.
 */
void lcdSendCommand(const struct device *const i2c_dev, char cmd);

/**
 * @brief Send data to the LCD.
 *
 * @param i2c_dev Pointer to the I2C device.
 * @param data Data byte to send.
 */
void lcdSendData(const struct device *const i2c_dev, char data);

/**
 * @brief Clear the LCD display.
 *
 * @param i2c_dev Pointer to the I2C device.
 */
void lcdClear(const struct device *const i2c_dev);

/**
 * @brief Set the cursor position on the LCD.
 *
 * @param i2c_dev Pointer to the I2C device.
 * @param row Row number (0 or 1).
 * @param col Column number.
 */
void lcdPutCur(const struct device *const i2c_dev, int row, int col);

/**
 * @brief Initialize the LCD.
 *
 * @param i2c_dev Pointer to the I2C device.
 */
void lcdInit(const struct device *const i2c_dev);

/**
 * @brief Send a string to the LCD.
 *
 * @param i2c_dev Pointer to the I2C device.
 * @param str Null-terminated string to send.
 */
void lcdSendString(const struct device *const i2c_dev, char *str);

#endif // I2C_LCD_H

