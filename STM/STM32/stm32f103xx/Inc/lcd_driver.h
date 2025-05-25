/*
 * lcd_driver.h
 *
 *  Created on: Apr 26, 2025
 *      Author: tr6r2
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_


#include <stm32f103xx.h>
#include <i2c_driver.h>

#define LCD_EN 0x04  // Enable bit
#define LCD_RW 0x02  // Read/Write bit
#define LCD_RS 0x01  // Register select bit
#define LCD_BL 0x08  // Backlight

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
uint8_t LCD_IsConnected(LCD_Handle_t *hlcd);
void lcd_i2c_data(I2C_Handle_t *hi2c, uint8_t address, unsigned char data);
void lcd_i2c_cmd(I2C_Handle_t *hi2c, uint8_t address, unsigned char data);
void lcd_i2c_init(I2C_Handle_t *hi2c, uint8_t address);
void lcd_i2c_send(I2C_Handle_t *hi2c, uint8_t address, char *str);
void lcd_i2c_send_data(I2C_Handle_t *hi2c, uint8_t address, uint8_t data);
void lcd_i2c_msg(I2C_Handle_t *hi2c, uint8_t address, unsigned char line, unsigned char pos, char *msg);

#endif /* LCD_DRIVER_H_ */
