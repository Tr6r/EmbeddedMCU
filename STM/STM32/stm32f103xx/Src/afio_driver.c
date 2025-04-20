/*
 * afio_driver.c
 *
 *  Created on: Apr 16, 2025
 *      Author: tr6r2
 */

#include"afio_driver.h"

void AFIO_Init(AFIO_Handle_t *hAFIOx) {
	if (hAFIOx->Config.Function == AFIO_FUNC_EXTI) {
		AFIO_EN_CLOCK();
		uint8_t regIndex = hAFIOx->Config.EXTI_Line / 4; // Tính toán chỉ mục của thanh ghi EXTICR
		uint8_t pinPos = (hAFIOx->Config.EXTI_Line % 4) * 4; // Vị trí bit của pin trong thanh ghi

		// Xóa các bit đã có trong thanh ghi
		AFIO->EXTICR[regIndex] &= ~(0xF << pinPos);

		// Đặt cổng GPIO vào thanh ghi tương ứng với pin
		AFIO->EXTICR[regIndex] |= (hAFIOx->Config.GPIO_PortSrc << pinPos);
	}
}
