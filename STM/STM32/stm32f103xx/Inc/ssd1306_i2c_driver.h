/*
 * ssd1306_i2c.h
 *
 *  Created on: Aug 3, 2025
 *      Author: tr6r2
 */

#ifndef SSD1306_I2C_DRIVER_H_
#define SSD1306_I2C_DRIVER_H_


#include <stm32f103xx.h>
#include <i2c_driver.h>

#define SSD1306_WIDTH     128
#define SSD1306_HEIGHT     32
#define SSD1306_BUFFER_SIZE  (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

typedef struct {
    uint8_t Address;       // I2C address (e.g. 0x3C)
    uint8_t Width;
    uint8_t Height;

} SSD1306_Config_t;



typedef struct {
    I2C_Handle_t *Instance;       // Pointer to your I2C driver (I2C1, I2C2, ...)
    SSD1306_Config_t Config;
    uint8_t Buffer[SSD1306_BUFFER_SIZE];
} SSD1306_Handle_t;

// Public API
void SSD1306_Init(SSD1306_Handle_t *hssd);
void SSD1306_Update(SSD1306_Handle_t *hssd);
void SSD1306_Clear(SSD1306_Handle_t *hssd);
void SSD1306_DrawPixel(SSD1306_Handle_t *hssd, uint8_t x, uint8_t y, uint8_t color);
void SSD1306_DrawChar(SSD1306_Handle_t *hssd, uint8_t x, uint8_t y, char c, uint8_t color);

void SSD1306_WriteString(SSD1306_Handle_t *hssd, uint8_t x, uint8_t y, const char *str, uint8_t color);

#endif /* SSD1306_I2C_DRIVER_H_ */
