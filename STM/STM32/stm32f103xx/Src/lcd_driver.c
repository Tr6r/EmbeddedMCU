/*
 * lcd_driver.c
 *
 *  Created on: Apr 26, 2025
 *      Author: tr6r2
 */

#include <lcd_driver.h>


void LCD_Delay(volatile uint32_t time) {
    while(time--);
}

void LCD_SendCommand(LCD_Handle_t *hlcd, uint8_t cmd) {
    uint8_t high_nibble = (cmd & 0xF0);            // 4 bit cao
    uint8_t low_nibble = (cmd << 4) & 0xF0;         // 4 bit thấp

    LCD_SendHalf(hlcd, high_nibble | 0x00); // RS = 0 (lệnh)
    LCD_SendHalf(hlcd, low_nibble | 0x00);
}

void LCD_SetCursor(LCD_Handle_t *hlcd, uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54}; // cho LCD 4 dòng
    if (row > 3) row = 3; // bảo vệ
    LCD_SendCommand(hlcd, 0x80 | (col + row_offsets[row]));
}
 void LCD_SendHalf(LCD_Handle_t *hlcd, uint8_t data) {
    uint8_t temp;

    // Gửi nửa byte dữ liệu (4 bit cao) với Enable = 1
    temp = (data & 0xF0) | 0x0C;  // Lấy 4 bit cao và set Enable = 1
    I2C_Write(hlcd->Instance, hlcd->Config.Address, &temp, 1);  // Gửi dữ liệu qua I2C

    // Delay cho LCD xử lý lệnh
    LCD_Delay(50000);

    // Gửi nửa byte dữ liệu (4 bit cao) với Enable = 0
    temp &= 0xFB;  // Set Enable = 0
    State_t ret = I2C_Write(hlcd->Instance, hlcd->Config.Address, &temp, 1);  // Gửi dữ liệu qua I2C
    if (ret == STATE_OK)
    {
        LCD_Delay(100000);
    }
    else     LCD_Delay(100000);

}


void LCD_SendData(LCD_Handle_t *hlcd, uint8_t data) {
    uint8_t high_nibble = (data & 0xF0);      // Lấy 4 bit cao của data
    uint8_t low_nibble = (data << 4) & 0xF0;  // Lấy 4 bit thấp của data

    // Gửi phần 4 bit cao
    LCD_SendHalf(hlcd, high_nibble | 0x01); // RS = 1 (data)
    // Gửi phần 4 bit thấp
    LCD_SendHalf(hlcd, low_nibble | 0x01);  // RS = 1 (data)
}


void LCD_SendString(LCD_Handle_t *hlcd, uint8_t col, uint8_t row, const char *str) {
    LCD_SetCursor(hlcd, col, row);
    while(*str) {
        LCD_SendData(hlcd, (uint8_t)(*str));
        str++;
    }
}

void LCD_Init(LCD_Handle_t *hlcd) {
    LCD_Delay(100000); // Delay ~50ms (tùy tốc độ CPU, cần chỉnh nếu nhanh chậm quá)

    LCD_SendCommand(hlcd, 0x33);
    LCD_SendCommand(hlcd, 0x32);
    LCD_SendCommand(hlcd, 0x28);
    LCD_SendCommand(hlcd, 0x0C);
    LCD_SendCommand(hlcd, 0x06);
    LCD_SendCommand(hlcd, 0x01);
    LCD_Delay(100000); // Delay ~5ms sau clear màn hình

}
