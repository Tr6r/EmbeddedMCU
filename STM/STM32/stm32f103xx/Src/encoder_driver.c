/*
 * encoder_driver.c
 *
 *  Created on: May 17, 2025
 *      Author: tr6r2
 */

#include <encoder_driver.h>
int16_t ENC_Read(ENC_Handle_t *hEnc) {
    // Đọc giá trị bộ đếm CNT của timer
    int16_t cnt = (int16_t)(hEnc->htim->Instance->CNT);
    hEnc->Config.count = cnt;  // Lưu vào struct
    return cnt;
}
