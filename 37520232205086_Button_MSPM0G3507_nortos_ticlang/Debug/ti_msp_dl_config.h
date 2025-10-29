/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     32000000



/* Defines for LED_rgb */
#define LED_rgb_INST                                                       TIMA0
#define LED_rgb_INST_IRQHandler                                 TIMA0_IRQHandler
#define LED_rgb_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define LED_rgb_INST_CLK_FREQ                                           16000000
/* GPIO defines for channel 0 */
#define GPIO_LED_rgb_C0_PORT                                               GPIOA
#define GPIO_LED_rgb_C0_PIN                                        DL_GPIO_PIN_8
#define GPIO_LED_rgb_C0_IOMUX                                    (IOMUX_PINCM19)
#define GPIO_LED_rgb_C0_IOMUX_FUNC                   IOMUX_PINCM19_PF_TIMA0_CCP0
#define GPIO_LED_rgb_C0_IDX                                  DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_LED_rgb_C1_PORT                                               GPIOB
#define GPIO_LED_rgb_C1_PIN                                       DL_GPIO_PIN_20
#define GPIO_LED_rgb_C1_IOMUX                                    (IOMUX_PINCM48)
#define GPIO_LED_rgb_C1_IOMUX_FUNC                   IOMUX_PINCM48_PF_TIMA0_CCP1
#define GPIO_LED_rgb_C1_IDX                                  DL_TIMER_CC_1_INDEX
/* GPIO defines for channel 2 */
#define GPIO_LED_rgb_C2_PORT                                               GPIOB
#define GPIO_LED_rgb_C2_PIN                                        DL_GPIO_PIN_0
#define GPIO_LED_rgb_C2_IOMUX                                    (IOMUX_PINCM12)
#define GPIO_LED_rgb_C2_IOMUX_FUNC                   IOMUX_PINCM12_PF_TIMA0_CCP2
#define GPIO_LED_rgb_C2_IDX                                  DL_TIMER_CC_2_INDEX




/* Defines for TIMER_QEI */
#define TIMER_QEI_INST                                                     TIMG8
#define TIMER_QEI_INST_IRQHandler                               TIMG8_IRQHandler
#define TIMER_QEI_INST_INT_IRQN                                 (TIMG8_INT_IRQn)
/* Pin configuration defines for TIMER_QEI PHA Pin */
#define GPIO_TIMER_QEI_PHA_PORT                                            GPIOB
#define GPIO_TIMER_QEI_PHA_PIN                                     DL_GPIO_PIN_6
#define GPIO_TIMER_QEI_PHA_IOMUX                                 (IOMUX_PINCM23)
#define GPIO_TIMER_QEI_PHA_IOMUX_FUNC                IOMUX_PINCM23_PF_TIMG8_CCP0
/* Pin configuration defines for TIMER_QEI PHB Pin */
#define GPIO_TIMER_QEI_PHB_PORT                                            GPIOB
#define GPIO_TIMER_QEI_PHB_PIN                                     DL_GPIO_PIN_7
#define GPIO_TIMER_QEI_PHB_IOMUX                                 (IOMUX_PINCM24)
#define GPIO_TIMER_QEI_PHB_IOMUX_FUNC                IOMUX_PINCM24_PF_TIMG8_CCP1


/* Defines for TIMER_MS */
#define TIMER_MS_INST                                                    (TIMG6)
#define TIMER_MS_INST_IRQHandler                                TIMG6_IRQHandler
#define TIMER_MS_INST_INT_IRQN                                  (TIMG6_INT_IRQn)
#define TIMER_MS_INST_LOAD_VALUE                                          (999U)




/* Port definition for Pin Group GPIO_LEDS */
#define GPIO_LEDS_PORT                                                   (GPIOA)

/* Defines for LED_1: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_LEDS_LED_1_PIN                                      (DL_GPIO_PIN_0)
#define GPIO_LEDS_LED_1_IOMUX                                     (IOMUX_PINCM1)
/* Port definition for Pin Group GPIO_EC11 */
#define GPIO_EC11_PORT                                                   (GPIOA)

/* Defines for SW: GPIOA.18 with pinCMx 40 on package pin 11 */
// pins affected by this interrupt request:["SW"]
#define GPIO_EC11_INT_IRQN                                      (GPIOA_INT_IRQn)
#define GPIO_EC11_INT_IIDX                      (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_EC11_SW_IIDX                                   (DL_GPIO_IIDX_DIO18)
#define GPIO_EC11_SW_PIN                                        (DL_GPIO_PIN_18)
#define GPIO_EC11_SW_IOMUX                                       (IOMUX_PINCM40)
/* Port definition for Pin Group GPIO_OLED_13 */
#define GPIO_OLED_13_PORT                                                (GPIOA)

/* Defines for GPIO_SCK: GPIOA.31 with pinCMx 6 on package pin 39 */
#define GPIO_OLED_13_GPIO_SCK_PIN                               (DL_GPIO_PIN_31)
#define GPIO_OLED_13_GPIO_SCK_IOMUX                               (IOMUX_PINCM6)
/* Defines for GPIO_SDA: GPIOA.28 with pinCMx 3 on package pin 35 */
#define GPIO_OLED_13_GPIO_SDA_PIN                               (DL_GPIO_PIN_28)
#define GPIO_OLED_13_GPIO_SDA_IOMUX                               (IOMUX_PINCM3)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_LED_rgb_init(void);
void SYSCFG_DL_TIMER_QEI_init(void);
void SYSCFG_DL_TIMER_MS_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
