#include <ti/iqmath/include/IQmathLib.h>
#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "I2C_GPIO_25a.h"
#include "OLED13.h"
#include "OLED_BMP.h"
#include "mspm0g350x.h"
#include "dl_adc12.h"
#include "hw_adc12.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/m0p/dl_factoryregion.h>
/* clang-format off */
/* 中断号定义 */
#define ADC0_IRQn                         4
#define ADC1_IRQn                         5
#define ADC0_BASE    (0x40000000U)
#define ADC1_BASE    (0x40002000U)
#define ADC0         ((ADC12_Regs *)ADC0_BASE)
#define ADC1         ((ADC12_Regs *)ADC1_BASE)
/*
 * The following trim parameter are provided in the device datasheet in chapter
 * "Temperature Sensor"
 */
#define TEMP_TS_TRIM_C                                            ((uint32_t)30)


#define TEMP_TS_COEF_mV_C                                             (-555.55f)

#define ADC_VREF_VOLTAGE                                                  (3.3f)
#define ADC_BIT_RESOLUTION                                   ((uint32_t)(1)<<12)
#define TMP6131_R25     (_IQ15(10000.0f))        /* 10 kΩ        */
#define TMP6131_TCR     (_IQ15(0.0064f))         /* 6400 ppm/°C  */
#define TMP6131_VCC     (_IQ15(3.3f))            /* 分压供电     */
#define TMP6131_R_REF   (_IQ15(10000.0f))        /* 参考电阻     */
#define  ALARM_DELTA_IQ15   (_IQ15(-5.0f))            /*报警温差      */
#define ADC_VOLTAGE_RESOLUTION_V  (0.00081f)  // 转换为伏特

#define DL_ADC12_SVT_OFFSET                  ((uint32_t)0x555000 >> (uint32_t)2)
/* clang-format off */

volatile bool gCheckADC0;
volatile bool gCheckADC1;
volatile float gTemperatureDegC;
volatile float outgTemperatureDegC;
volatile bool gSampleExternalTemp = false;
volatile bool gAlarmActive = false;
volatile uint32_t gFlashCounter = 0;
//
/* 与片内温度代码保持同一格式 *************************************/
static _iq15 calc_TMP6131_Temp(uint32_t adcResult)
{
    _iq15 qVSample, qRtherm, qTsample;

    /* 1. ADC转换 */
    qVSample = _IQ15mpy(
                       (_IQ15(adcResult) - _IQ15(0.5f)),
                      _IQ15(ADC_VOLTAGE_RESOLUTION_V)
                  );

    /* 2. 计算电阻 */
    qRtherm = _IQ15div(
                   _IQ15mpy(qVSample, _IQ15(10000.0f)),
                   _IQ15(TMP6131_VCC) - qVSample);

    /* 3. 对于PTC热敏电阻，使用正确的温度计算公式 */
    // 方法1：线性近似（简单）
    // T = 25 + (R - R25) / (R25 * TCR)
    qTsample = _IQ15(25.0f) +
               _IQ15div(
                   (qRtherm - _IQ15(10000.0f)),
                   _IQ15mpy(_IQ15(10000.0f), _IQ15(TMP6131_TCR))
               );

    return qTsample;
}
int main(void)
{
    uint32_t adcResult;
    SYSCFG_DL_init();
    OLED_Init();

    NVIC_EnableIRQ(TIMER_1S_INST_INT_IRQN);  // 使能中断
    NVIC_EnableIRQ(TIMER_5086_INST_INT_IRQN);  // 使能中断
    NVIC->ISER[0U] = (uint32_t)(1UL << (((uint32_t)ADC0_IRQn) & 0x1FUL));
        NVIC->ISER[0U] = (uint32_t)(1UL << (((uint32_t)ADC1_IRQn) & 0x1FUL));

    DL_TimerA_startCounter(TIMER_1S_INST);   // 开始计时
    DL_TimerA_startCounter(TIMER_5086_INST);   // 开始计时
    //DL_GPIO_clearPins(GPIO_LEDs_PORT, GPIO_LEDs_LED_R_PIN | GPIO_LEDs_LED_G_PIN); // 先全灭
    _iq15 qVSample, qTsample, qTempDegF, qVTrim;

    qVTrim = _IQ15div(_IQ15mpy((_IQ15(DL_SYSCTL_getTempCalibrationConstant()) -
            _IQ15(0.5)), _IQ15(ADC_VREF_VOLTAGE)), ((uint32_t)(1) << 27));

    gCheckADC0 = false;
    gCheckADC1 = false;
    gSampleExternalTemp = false;
    /* 1. 把 PA15 的复用功能切到 ADC1_CH0 */
    DL_GPIO_initPeripheralInputFunction(
        GPIO_ADC12_1_IOMUX_C3,          // IOMUX_PINCM37
        GPIO_ADC12_1_IOMUX_C3_FUNC       // 复用功能编号

    );

    while (1) {
        // 内部温度采样
        ADC0->ULLMEM.CTL1 |=((uint32_t)0x00000100U);
        while (false == gCheckADC0) {
            __WFE();
        }

        gCheckADC0 = false;
        ADC1->ULLMEM.CTL1 &= ~(ADC12_CTL1_SC_START);
        uint32_t *pRegk = &ADC0->ULLMEM.MEMRES[0];
        adcResult =(uint16_t)(*(pRegk + DL_ADC12_SVT_OFFSET));

        qVSample = _IQ15div(_IQ15mpy((adcResult << 15) -
                _IQ15(0.5),_IQ15(ADC_VREF_VOLTAGE)), _IQ15(ADC_BIT_RESOLUTION));

        qTsample = _IQ15mpy(_IQ15(TEMP_TS_COEF_mV_C), (qVSample - qVTrim)) +
                            (TEMP_TS_TRIM_C << 15);

        qTempDegF = _IQ15mpy(qTsample , _IQ15(9.0/5.0)) + _IQ15(32.0);
        gTemperatureDegC = _IQ15toF(qTsample);

        OLED_ShowString(0, 0, "DYT Version:1 ");
                char line[32];
                snprintf(line, sizeof(line), "Int:%.2f C", gTemperatureDegC);
                OLED_ShowString(0, 2, line);
                //外部温度采样
                if (gSampleExternalTemp) {
                    gSampleExternalTemp = false;
                    ADC1->ULLMEM.CTL1 |=((uint32_t)0x00000100U);
                    while(false ==gCheckADC1)
                        __WFE();
                }

                gCheckADC1 = false;
                 pRegk = &ADC1->ULLMEM.MEMRES[1];
                   adcResult =(uint16_t)(*(pRegk + DL_ADC12_SVT_OFFSET));
                    _iq15 tmpIQ = calc_TMP6131_Temp(adcResult);
                     outgTemperatureDegC = _IQ15toF(tmpIQ);

                // 显示和报警逻辑
                snprintf(line, sizeof(line), "Int:%.2f C", gTemperatureDegC);
                OLED_ShowString(0, 2, line);

                snprintf(line, sizeof(line), "OUT:%.2f C", outgTemperatureDegC);
                OLED_ShowString(0, 4, line);
                snprintf(line, sizeof(line), "Alarm:%.2f C", outgTemperatureDegC+_IQ15toF(ALARM_DELTA_IQ15));
                OLED_ShowString(0,6,line);
                OLED_Refresh();
                        //报警
                /* 得到内外温度后 */
                _iq15 delta = gTemperatureDegC - outgTemperatureDegC;   // 浮点也可
                gAlarmActive = (delta >= ALARM_DELTA_IQ15);

}}

void ADC12_0_INST_IRQHandler(void)
{
    switch (((DL_ADC12_IIDX) ADC0->ULLMEM.CPU_INT.IIDX)) {
        case ADC12_CPU_INT_IIDX_STAT_MEMRESIFG0:
            gCheckADC0 = true;
            break;
        default:
            break;
    }
}
void ADC12_1_INST_IRQHandler(void)
{
    switch (((DL_ADC12_IIDX) ADC1->ULLMEM.CPU_INT.IIDX)) {
        case ADC12_CPU_INT_IIDX_STAT_MEMRESIFG1:
            gCheckADC1 = true;
            break;
        default:
            break;
    }
}
void TIMER_1S_INST_IRQHandler(void)
{
    DL_GPIO_togglePins(GPIO_LED1_PORT,GPIO_LED1_LED1_PIN);//LED1闪烁

    DL_TimerA_clearInterruptStatus(TIMER_1S_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);

}
void TIMER_5086_INST_IRQHandler(void)
{
    gSampleExternalTemp = true;      // 原有功能保留

    if (gAlarmActive) {              /* 快闪红灯 */
        static uint8_t cnt = 0;
        cnt++;
        DL_GPIO_clearPins(GPIO_LEDs_PORT, GPIO_LEDs_LED_G_PIN);
        if (cnt >= 100){            // 100 ≈ 1 s 翻转一次 → 300 Hz 快闪
            cnt = 0;
            DL_GPIO_togglePins(GPIO_LEDs_PORT, GPIO_LEDs_LED_R_PIN);
        }
        /* 绿灯灭 */

    }
    else {                           /* 常亮绿灯 */
        DL_GPIO_setPins(GPIO_LEDs_PORT,   GPIO_LEDs_LED_G_PIN);
        DL_GPIO_clearPins(GPIO_LEDs_PORT, GPIO_LEDs_LED_R_PIN);
    }

    DL_TimerA_clearInterruptStatus(TIMER_5086_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);
}
