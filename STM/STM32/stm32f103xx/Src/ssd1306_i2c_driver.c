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

const uint8_t Font5x7[][5] = {
    // '0'-'9'
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9

    // ':'
    {0x00, 0x36, 0x36, 0x00, 0x00}  // :
};
void SSD1306_DrawChar(SSD1306_Handle_t *hssd, uint8_t x, uint8_t y, char c, uint8_t color) {
    const uint8_t *bitmap = NULL;
    if(c >= '0' && c <= '9') {
        bitmap = Font5x7[c - '0'];
    } else if(c == ':') {
        bitmap = Font5x7[10]; // dấu ':' nằm ở vị trí 10
    } else {
        return; // không hỗ trợ ký tự khác
    }

    for(uint8_t i = 0; i < 5; i++) {
        uint8_t line = bitmap[i];
        for(uint8_t j = 0; j < 7; j++) {
            if(line & 0x01) SSD1306_DrawPixel(hssd, x + i, y + j, color);
            line >>= 1;
        }
    }
}
void SSD1306_WriteString(SSD1306_Handle_t *hssd, uint8_t x, uint8_t y, const char *str, uint8_t color) {
    while(*str) {
        SSD1306_DrawChar(hssd, x, y, *str, color);
        x += 6; // 5 pixel + 1 pixel khoảng cách
        str++;
    }
    SSD1306_Update(hssd); // cập nhật màn hình
}
