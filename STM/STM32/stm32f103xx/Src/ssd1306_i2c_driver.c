/*
 * ssd1306_i2c.c
 *
 *  Created on: Aug 3, 2025
 *      Author: tr6r2
 */


#include <ssd1306_i2c_driver.h>

static void SSD1306_SendCommand(SSD1306_Handle_t *hssd, uint8_t cmd) {
    uint8_t data[2] = {0x00, cmd};
    I2C_Write(hssd->Instance, hssd->Config.Address, data, 2);
}

static void SSD1306_SendData(SSD1306_Handle_t *hssd, uint8_t *data, size_t len) {
    uint8_t temp[len + 1];
    temp[0] = 0x40; // Control byte for data
    memcpy(&temp[1], data, len);
    I2C_Write(hssd->Instance, hssd->Config.Address, temp, len + 1);
}

void SSD1306_Init(SSD1306_Handle_t *hssd) {
    LCD_Delay(100); // hoặc Delay_ms

    SSD1306_SendCommand(hssd, 0xAE); // Display off
    SSD1306_SendCommand(hssd, 0x20); SSD1306_SendCommand(hssd, 0x00); // Horizontal addressing mode
    SSD1306_SendCommand(hssd, 0xB0);
    SSD1306_SendCommand(hssd, 0xC8);
    SSD1306_SendCommand(hssd, 0x00);
    SSD1306_SendCommand(hssd, 0x10);
    SSD1306_SendCommand(hssd, 0x40);
    SSD1306_SendCommand(hssd, 0x81); SSD1306_SendCommand(hssd, 0x7F);
    SSD1306_SendCommand(hssd, 0xA1);
    SSD1306_SendCommand(hssd, 0xA6);
    SSD1306_SendCommand(hssd, 0xA8); SSD1306_SendCommand(hssd, hssd->Config.Height - 1);
    SSD1306_SendCommand(hssd, 0xD3); SSD1306_SendCommand(hssd, 0x00);
    SSD1306_SendCommand(hssd, 0xD5); SSD1306_SendCommand(hssd, 0x80);
    SSD1306_SendCommand(hssd, 0xD9); SSD1306_SendCommand(hssd, 0xF1);
    SSD1306_SendCommand(hssd, 0xDA); SSD1306_SendCommand(hssd, 0x02);
    SSD1306_SendCommand(hssd, 0xDB); SSD1306_SendCommand(hssd, 0x40);
    SSD1306_SendCommand(hssd, 0x8D); SSD1306_SendCommand(hssd, 0x14);
    SSD1306_SendCommand(hssd, 0xAF); // Display on

    SSD1306_Clear(hssd);
    SSD1306_Update(hssd);
}

void SSD1306_Clear(SSD1306_Handle_t *hssd) {
    memset(hssd->Buffer, 0x00, SSD1306_BUFFER_SIZE);
}

void SSD1306_DrawPixel(SSD1306_Handle_t *hssd, uint8_t x, uint8_t y, uint8_t color) {
    if (x >= hssd->Config.Width || y >= hssd->Config.Height) return;
    uint16_t index = x + (y / 8) * hssd->Config.Width;

    if (color)
        hssd->Buffer[index] |= (1 << (y % 8));
    else
        hssd->Buffer[index] &= ~(1 << (y % 8));
}

void SSD1306_Update(SSD1306_Handle_t *hssd) {
    for (uint8_t page = 0; page < (hssd->Config.Height / 8); page++) {
        SSD1306_SendCommand(hssd, 0xB0 + page);     // Page start
        SSD1306_SendCommand(hssd, 0x00);            // Lower col
        SSD1306_SendCommand(hssd, 0x10);            // Higher col
        SSD1306_SendData(hssd, &hssd->Buffer[page * hssd->Config.Width], hssd->Config.Width);
    }
}
