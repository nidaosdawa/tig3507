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



/* Defines for TIMER_5086 */
#define TIMER_5086_INST                                                  (TIMA1)
#define TIMER_5086_INST_IRQHandler                              TIMA1_IRQHandler
#define TIMER_5086_INST_INT_IRQN                                (TIMA1_INT_IRQn)
#define TIMER_5086_INST_LOAD_VALUE                                        (392U)
/* Defines for TIMER_1S */
#define TIMER_1S_INST                                                    (TIMA0)
#define TIMER_1S_INST_IRQHandler                                TIMA0_IRQHandler
#define TIMER_1S_INST_INT_IRQN                                  (TIMA0_INT_IRQn)
#define TIMER_1S_INST_LOAD_VALUE                                        (39999U)




/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC0
#define ADC12_0_INST_IRQHandler                                  ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC0_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_0_ADCMEM_0_REF_VOLTAGE_V                                       3.3

/* Defines for ADC12_1 */
#define ADC12_1_INST                                                        ADC1
#define ADC12_1_INST_IRQHandler                                  ADC1_IRQHandler
#define ADC12_1_INST_INT_IRQN                                    (ADC1_INT_IRQn)
#define ADC12_1_ADCMEM_0                                      DL_ADC12_MEM_IDX_1
#define ADC12_1_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_1_ADCMEM_0_REF_VOLTAGE_V                                       3.3
#define GPIO_ADC12_1_C3_PORT                                               GPIOA
#define GPIO_ADC12_1_C3_PIN                                       DL_GPIO_PIN_18
#define GPIO_ADC12_1_IOMUX_C3                                    (IOMUX_PINCM40)
#define GPIO_ADC12_1_IOMUX_C3_FUNC                (IOMUX_PINCM40_PF_UNCONNECTED)



/* Port definition for Pin Group GPIO_LED1 */
#define GPIO_LED1_PORT                                                   (GPIOA)

/* Defines for LED1: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_LED1_LED1_PIN                                       (DL_GPIO_PIN_0)
#define GPIO_LED1_LED1_IOMUX                                      (IOMUX_PINCM1)
/* Port definition for Pin Group GPIO_OLED_13 */
#define GPIO_OLED_13_PORT                                                (GPIOA)

/* Defines for GPIO_SDA: GPIOA.28 with pinCMx 3 on package pin 35 */
#define GPIO_OLED_13_GPIO_SDA_PIN                               (DL_GPIO_PIN_28)
#define GPIO_OLED_13_GPIO_SDA_IOMUX                               (IOMUX_PINCM3)
/* Defines for GPIO_SCK: GPIOA.31 with pinCMx 6 on package pin 39 */
#define GPIO_OLED_13_GPIO_SCK_PIN                               (DL_GPIO_PIN_31)
#define GPIO_OLED_13_GPIO_SCK_IOMUX                               (IOMUX_PINCM6)
/* Port definition for Pin Group GPIO_LEDs */
#define GPIO_LEDs_PORT                                                   (GPIOB)

/* Defines for LED_R: GPIOB.26 with pinCMx 57 on package pin 28 */
#define GPIO_LEDs_LED_R_PIN                                     (DL_GPIO_PIN_26)
#define GPIO_LEDs_LED_R_IOMUX                                    (IOMUX_PINCM57)
/* Defines for LED_G: GPIOB.27 with pinCMx 58 on package pin 29 */
#define GPIO_LEDs_LED_G_PIN                                     (DL_GPIO_PIN_27)
#define GPIO_LEDs_LED_G_IOMUX                                    (IOMUX_PINCM58)




/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMER_5086_init(void);
void SYSCFG_DL_TIMER_1S_init(void);
void SYSCFG_DL_ADC12_0_init(void);
void SYSCFG_DL_ADC12_1_init(void);



bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
