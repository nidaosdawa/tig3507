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


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                      8000000




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
/* Pin configuration defines for TIMER_QEI IDX Pin */
#define GPIO_TIMER_QEI_IDX_PORT                                            GPIOA
#define GPIO_TIMER_QEI_IDX_PIN                                     DL_GPIO_PIN_1
#define GPIO_TIMER_QEI_IDX_IOMUX                                  (IOMUX_PINCM2)
#define GPIO_TIMER_QEI_IDX_IOMUX_FUNC                  IOMUX_PINCM2_PF_TIMG8_IDX


/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA0)
#define TIMER_0_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                           (399U)
#define TIMER_0_INST_PUB_0_CH                                                (1)
/* Defines for TIMER_MS */
#define TIMER_MS_INST                                                    (TIMA1)
#define TIMER_MS_INST_IRQHandler                                TIMA1_IRQHandler
#define TIMER_MS_INST_INT_IRQN                                  (TIMA1_INT_IRQn)
#define TIMER_MS_INST_LOAD_VALUE                                          (999U)




/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC0
#define ADC12_0_INST_IRQHandler                                  ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC0_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_0_ADCMEM_0_REF_VOLTAGE_V                                       3.3
#define GPIO_ADC12_0_C2_PORT                                               GPIOA
#define GPIO_ADC12_0_C2_PIN                                       DL_GPIO_PIN_25
#define GPIO_ADC12_0_IOMUX_C2                                    (IOMUX_PINCM55)
#define GPIO_ADC12_0_IOMUX_C2_FUNC                (IOMUX_PINCM55_PF_UNCONNECTED)



/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (0)
#define DAC12_INST_DMA_TRIGGER                          (DMA_DAC0_EVT_BD_1_TRIG)


/* Port definition for Pin Group GPIO_EC11 */
#define GPIO_EC11_PORT                                                   (GPIOA)

/* Defines for SW: GPIOA.18 with pinCMx 40 on package pin 11 */
// pins affected by this interrupt request:["SW"]
#define GPIO_EC11_INT_IRQN                                      (GPIOA_INT_IRQn)
#define GPIO_EC11_INT_IIDX                      (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_EC11_SW_IIDX                                   (DL_GPIO_IIDX_DIO18)
#define GPIO_EC11_SW_PIN                                        (DL_GPIO_PIN_18)
#define GPIO_EC11_SW_IOMUX                                       (IOMUX_PINCM40)
/* Port definition for Pin Group GPIO_LED */
#define GPIO_LED_PORT                                                    (GPIOA)

/* Defines for PIN_0: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_LED_PIN_0_PIN                                       (DL_GPIO_PIN_0)
#define GPIO_LED_PIN_0_IOMUX                                      (IOMUX_PINCM1)
/* Port definition for Pin Group GPIO_SET */
#define GPIO_SET_PORT                                                    (GPIOB)

/* Defines for PIN_1: GPIOB.22 with pinCMx 50 on package pin 21 */
#define GPIO_SET_PIN_1_PIN                                      (DL_GPIO_PIN_22)
#define GPIO_SET_PIN_1_IOMUX                                     (IOMUX_PINCM50)
/* Port definition for Pin Group GPIO_OLED_13 */
#define GPIO_OLED_13_PORT                                                (GPIOA)

/* Defines for GPIO_SDA: GPIOA.28 with pinCMx 3 on package pin 35 */
#define GPIO_OLED_13_GPIO_SDA_PIN                               (DL_GPIO_PIN_28)
#define GPIO_OLED_13_GPIO_SDA_IOMUX                               (IOMUX_PINCM3)
/* Defines for GPIO_SCK: GPIOA.31 with pinCMx 6 on package pin 39 */
#define GPIO_OLED_13_GPIO_SCK_PIN                               (DL_GPIO_PIN_31)
#define GPIO_OLED_13_GPIO_SCK_IOMUX                               (IOMUX_PINCM6)



/* Defines for DAC12 */
#define DAC12_IRQHandler                                         DAC0_IRQHandler
#define DAC12_INT_IRQN                                           (DAC0_INT_IRQn)
#define GPIO_DAC12_OUT_PORT                                                GPIOA
#define GPIO_DAC12_OUT_PIN                                        DL_GPIO_PIN_15
#define GPIO_DAC12_IOMUX_OUT                                     (IOMUX_PINCM37)
#define GPIO_DAC12_IOMUX_OUT_FUNC                   IOMUX_PINCM37_PF_UNCONNECTED


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMER_QEI_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_TIMER_MS_init(void);
void SYSCFG_DL_ADC12_0_init(void);
void SYSCFG_DL_DMA_init(void);

void SYSCFG_DL_DAC12_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
