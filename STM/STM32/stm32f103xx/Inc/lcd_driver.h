/*
 * lcd_driver.h
 *
 *  Created on: Apr 26, 2025
 *      Author: tr6r2
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_


#include <i2c_driver.h>

typedef struct {
    uint8_t Address;           // Địa chỉ I2C của LCD (ví dụ 0x27, 0x3F)
    uint8_t BacklightState;    // Trạng thái đèn nền (ON = 1, OFF = 0)
} LCD_Config_t;

typedef struct {
    I2C_Handle_t *Instance;  // Con trỏ tới I2C1, I2C2,...
    LCD_Config_t Config;  // Cấu hình LCD (Address, Columns, Rows,...)

} LCD_Handle_t;

void LCD_Init(LCD_Handle_t *hlcd);
void LCD_SendString(LCD_Handle_t *hlcd, uint8_t col, uint8_t row, const char *str);

#endif /* LCD_DRIVER_H_ */
