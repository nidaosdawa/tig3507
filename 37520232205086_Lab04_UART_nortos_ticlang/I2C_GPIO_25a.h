#ifndef __I2C_GPIO_25a_H
#define __I2C_GPIO_25a_H

#include "ti_msp_dl_config.h"


#define OLED_I2C_ADR    (0x78)  // OLED 模块 I2C 器件地址

/* Port definition for Pin Group GPIO_OLED13 */
#define GPIO_OLED13_PORT                                                 (GPIOA)

/* Defines for GPIO_SCK: GPIOA.31 with pinCMx 6 on package pin 39 */
#define GPIO_OLED13_GPIO_SCK_PIN                                (DL_GPIO_PIN_31)
#define GPIO_OLED13_GPIO_SCK_IOMUX                                (IOMUX_PINCM6)
/* Defines for GPIO_SDA: GPIOA.28 with pinCMx 3 on package pin 35 */
#define GPIO_OLED13_GPIO_SDA_PIN                                (DL_GPIO_PIN_28)
#define GPIO_OLED13_GPIO_SDA_IOMUX                                (IOMUX_PINCM3)


extern void I2CB1_Init(void);   // 初始化 I2C 模块
// OLED 控制用函数
extern void OLED_WR_Byte(uint8_t dat,uint8_t cmd);  

#endif

// ==========================================================================
// End of file.
