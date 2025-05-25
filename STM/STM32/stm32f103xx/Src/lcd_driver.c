/*
 * lcd_driver.c
 *
 *  Created on: Apr 26, 2025
 *      Author: tr6r2
 */

#include <lcd_driver.h>

void LCD_Delay(volatile uint32_t time) {
	while (time--)
		;
}

//void LCD_SendCommand(LCD_Handle_t *hlcd, uint8_t cmd) {
//	uint8_t high_nibble = (cmd & 0xF0);
//	uint8_t low_nibble = (cmd << 4) & 0xF0;
//
//	LCD_SendHalf(hlcd, high_nibble); // RS = 0
//	LCD_SendHalf(hlcd, low_nibble);  // RS = 0
//}
//
//void LCD_SetCursor(LCD_Handle_t *hlcd, uint8_t col, uint8_t row) {
//	uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 }; // cho LCD 4 dòng
//	if (row > 3)
//		row = 3; // bảo vệ
//	LCD_SendCommand(hlcd, 0x80 | (col + row_offsets[row]));
//}
//void LCD_SendHalf(LCD_Handle_t *hlcd, uint8_t data) {
//	uint8_t temp;
//	State_t status;
//
//	// Bước 1: Gửi data với EN = 1
//	temp = data | LCD_BL | LCD_EN;
//	status = I2C_Write(hlcd->Instance, hlcd->Config.Address, &temp, 1);
//	if (status != STATE_OK) {
//		// Xử lý lỗi I2C
//		return;
//	}
//	LCD_Delay(1000);
//
//	// Bước 2: Gửi data với EN = 0
//	temp = data | LCD_BL;
//	status = I2C_Write(hlcd->Instance, hlcd->Config.Address, &temp, 1);
//	if (status != STATE_OK) {
//		// Xử lý lỗi I2C
//		return;
//	}
//	LCD_Delay(5000);
//}
//
//void LCD_SendData(LCD_Handle_t *hlcd, uint8_t data) {
//	uint8_t high_nibble = (data & 0xF0);
//	uint8_t low_nibble = (data << 4) & 0xF0;
//
//	LCD_SendHalf(hlcd, high_nibble | LCD_RS); // RS = 1 (data)
//	LCD_SendHalf(hlcd, low_nibble | LCD_RS); // RS = 1 (data)
//}
//
//void LCD_SendString(LCD_Handle_t *hlcd, uint8_t col, uint8_t row,
//		const char *str) {
////    LCD_SetCursor(hlcd, col, row);
//	LCD_SendCommand(hlcd, 0x80); // Move cursor to beginning of first line
//	while (*str) {
//		LCD_SendData(hlcd, (uint8_t) (*str));
//		str++;
//	}
//}
//uint8_t LCD_IsConnected(LCD_Handle_t *hlcd) {
//	uint8_t dummy = 0;
//	return (I2C_Write(hlcd->Instance, hlcd->Config.Address, &dummy, 1)
//			== STATE_OK) ? 1 : 0;
//}
//void LCD_Init(LCD_Handle_t *hlcd) {
//    // Đợi LCD khởi động
//    LCD_Delay(150000); // ~15ms
//
//    // Gửi 0x03 ba lần (khởi tạo 8-bit interface)
//    LCD_SendHalf(hlcd, 0x30);
//    LCD_Delay(50000);  // ~4.1ms
//    LCD_SendHalf(hlcd, 0x30);
//    LCD_Delay(10000);   // ~100us
//    LCD_SendHalf(hlcd, 0x30);
//    LCD_Delay(10000);
//
//    // Chuyển sang chế độ 4-bit
//    LCD_SendHalf(hlcd, 0x20);
//    LCD_Delay(10000);
//
//    // Cấu hình hiển thị
//    LCD_SendCommand(hlcd, 0x28); // 4-bit, 2 lines, 5x8 font
//    LCD_SendCommand(hlcd, 0x0C); // Display ON, cursor OFF, blink OFF
//    LCD_SendCommand(hlcd, 0x06); // Entry mode set
//    LCD_SendCommand(hlcd, 0x01); // Clear display
//    LCD_Delay(20000);  // ~2ms
//}
void lcd_i2c_data(I2C_Handle_t *hi2c, uint8_t address, unsigned char data)
{
	uint8_t temp;

	// Send high nibble with control bits (RS=1, RW=0, E=0 -> 0x09; E=1 -> 0x0D)
	temp = 0x09;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	temp = 0x0D;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	// Send high nibble data (upper 4 bits) with E=1
	temp = (data & 0x00F0) | 0x0D;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	// E=0
	temp = (data & 0xF0) | 0x09;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	// Send low nibble with E=1
	temp = 0x0D;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	temp = ((data << 4) & 0x00F0) | 0x0D;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	// E=0
	temp = ((data << 4) & 0x00F0) | 0x09;
	I2C_Write(hi2c, address, &temp, 1);
}

void lcd_i2c_cmd(I2C_Handle_t *hi2c, uint8_t address, unsigned char data)
{
	uint8_t temp;

	// RS=0, RW=0, E=0 -> 0x08; E=1 -> 0x0C
	temp = 0x08;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	temp = 0x0C;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	// High nibble data + E=1
	temp = (data & 0x00F0) | 0x0C;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	// E=0
	temp = (data & 0x00F0) | 0x08;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	// E=1
	temp = 0x0C;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	// Low nibble data + E=1
	temp = ((data << 4) & 0x00F0) | 0x0C;
	I2C_Write(hi2c, address, &temp, 1);
	LCD_Delay(100);

	// E=0
	temp = ((data << 4) & 0x00F0) | 0x08;
	I2C_Write(hi2c, address, &temp, 1);
}

void lcd_i2c_init(I2C_Handle_t *hi2c, uint8_t address)
{
	// i2c_init(hi2c, i2c_FM); // gọi init nếu cần
	LCD_Delay(100);

	uint8_t cmds[] = {0x08, 0x0C, 0x3C, 0x38,
					  0x08, 0x0C, 0x3C, 0x38,
					  0x08, 0x0C, 0x3C, 0x38,
					  0x08, 0x0C, 0x2C, 0x28};
	for(int i=0; i<sizeof(cmds); i++) {
		I2C_Write(hi2c, address, &cmds[i], 1);
		LCD_Delay(100);
	}

	lcd_i2c_cmd(hi2c, address, 0x2C); // 4 bit / 2 line mode
	LCD_Delay(100);

	lcd_i2c_cmd(hi2c, address, 0x0C); // Display ON
	LCD_Delay(100);

	lcd_i2c_cmd(hi2c, address, 0x01); // Clear display
	LCD_Delay(100);

	lcd_i2c_cmd(hi2c, address, 0x02); // Return home
	LCD_Delay(100);
}

void lcd_i2c_send(I2C_Handle_t *hi2c, uint8_t address, char *str)
{
	for(int i=0; str[i]; i++) {
		lcd_i2c_data(hi2c, address, str[i]);
		LCD_Delay(100);
	}
}
void lcd_i2c_send_data(I2C_Handle_t *hi2c, uint8_t address, uint8_t data)
{
    uint8_t high = data & 0xF0;
    uint8_t low  = (data << 4) & 0xF0;

    uint8_t sequence[4] = {
        high | 0x0D, // RS=1, EN=1, BL=1
        high | 0x09, // RS=1, EN=0, BL=1
        low  | 0x0D, // RS=1, EN=1, BL=1
        low  | 0x09  // RS=1, EN=0, BL=1
    };

    for(int i = 0; i < 4; i++) {
        I2C_Write(hi2c, address, &sequence[i], 1);
        LCD_Delay(10); // nhỏ là đủ rồi
    }
}

void lcd_i2c_msg(I2C_Handle_t *hi2c, uint8_t address, unsigned char line, unsigned char pos, char *msg)
{
    uint8_t pos_addr = (line == 1) ? 0x00 : 0x40;
    lcd_i2c_cmd(hi2c, address, 0x80 + pos_addr + pos); // Đặt vị trí con trỏ
    LCD_Delay(20);

    while (*msg) {
        lcd_i2c_send_data(hi2c, address, *msg++); // Gửi từng ký tự
    }
}

