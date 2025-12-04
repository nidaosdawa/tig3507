/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

DL_TimerG_backupConfig gTIMER_QEIBackup;
DL_TimerA_backupConfig gTIMER_0Backup;
DL_TimerA_backupConfig gTIMER_MSBackup;

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_TIMER_QEI_init();
    SYSCFG_DL_TIMER_0_init();
    SYSCFG_DL_TIMER_MS_init();
    SYSCFG_DL_ADC12_0_init();
    SYSCFG_DL_DMA_init();
    SYSCFG_DL_DAC12_init();
    /* Ensure backup structures have no valid state */
    gTIMER_QEIBackup.backupRdy  = false;
    gTIMER_0Backup.backupRdy    = false;
    gTIMER_MSBackup.backupRdy   = false;

}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

    retStatus &= DL_TimerG_saveConfiguration(TIMER_QEI_INST, &gTIMER_QEIBackup);
    retStatus &= DL_TimerA_saveConfiguration(TIMER_0_INST, &gTIMER_0Backup);
    retStatus &= DL_TimerA_saveConfiguration(TIMER_MS_INST, &gTIMER_MSBackup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

    retStatus &= DL_TimerG_restoreConfiguration(TIMER_QEI_INST, &gTIMER_QEIBackup, true);
    retStatus &= DL_TimerA_restoreConfiguration(TIMER_0_INST, &gTIMER_0Backup, false);
    retStatus &= DL_TimerA_restoreConfiguration(TIMER_MS_INST, &gTIMER_MSBackup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerG_reset(TIMER_QEI_INST);
    DL_TimerA_reset(TIMER_0_INST);
    DL_TimerA_reset(TIMER_MS_INST);
    DL_ADC12_reset(ADC12_0_INST);

    DL_DAC12_reset(DAC0);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerG_enablePower(TIMER_QEI_INST);
    DL_TimerA_enablePower(TIMER_0_INST);
    DL_TimerA_enablePower(TIMER_MS_INST);
    DL_ADC12_enablePower(ADC12_0_INST);

    DL_DAC12_enablePower(DAC0);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralAnalogFunction(GPIO_HFXIN_IOMUX);
    DL_GPIO_initPeripheralAnalogFunction(GPIO_HFXOUT_IOMUX);

    DL_GPIO_initPeripheralInputFunction(GPIO_TIMER_QEI_PHA_IOMUX,GPIO_TIMER_QEI_PHA_IOMUX_FUNC);
    DL_GPIO_initPeripheralInputFunction(GPIO_TIMER_QEI_PHB_IOMUX,GPIO_TIMER_QEI_PHB_IOMUX_FUNC);
    DL_GPIO_initPeripheralInputFunction(GPIO_TIMER_QEI_IDX_IOMUX,GPIO_TIMER_QEI_IDX_IOMUX_FUNC);

    DL_GPIO_initDigitalInputFeatures(GPIO_EC11_SW_IOMUX,
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
         DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalOutput(GPIO_LED_PIN_0_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_SET_PIN_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_OLED_13_GPIO_SDA_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_OLED_13_GPIO_SCK_IOMUX);

    DL_GPIO_clearPins(GPIOA, GPIO_LED_PIN_0_PIN |
        GPIO_OLED_13_GPIO_SDA_PIN |
        GPIO_OLED_13_GPIO_SCK_PIN);
    DL_GPIO_enableOutput(GPIOA, GPIO_LED_PIN_0_PIN |
        GPIO_OLED_13_GPIO_SDA_PIN |
        GPIO_OLED_13_GPIO_SCK_PIN);
    DL_GPIO_setUpperPinsPolarity(GPIOA, DL_GPIO_PIN_18_EDGE_RISE);
    DL_GPIO_clearInterruptStatus(GPIOA, GPIO_EC11_SW_PIN);
    DL_GPIO_enableInterrupt(GPIOA, GPIO_EC11_SW_PIN);
    DL_GPIO_clearPins(GPIO_SET_PORT, GPIO_SET_PIN_1_PIN);
    DL_GPIO_enableOutput(GPIO_SET_PORT, GPIO_SET_PIN_1_PIN);

}


static const DL_SYSCTL_SYSPLLConfig gSYSPLLConfig = {
    .inputFreq              = DL_SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ,
    .rDivClk2x              = 1,
    .rDivClk1               = 0,
    .rDivClk0               = 4,
    .enableCLK2x            = DL_SYSCTL_SYSPLL_CLK2X_DISABLE,
    .enableCLK1             = DL_SYSCTL_SYSPLL_CLK1_DISABLE,
    .enableCLK0             = DL_SYSCTL_SYSPLL_CLK0_ENABLE,
    .sysPLLMCLK             = DL_SYSCTL_SYSPLL_MCLK_CLK0,
    .sysPLLRef              = DL_SYSCTL_SYSPLL_REF_HFCLK,
    .qDiv                   = 1,
    .pDiv                   = DL_SYSCTL_SYSPLL_PDIV_1
};
SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
    DL_SYSCTL_enableSleepOnExit();
    DL_SYSCTL_setFlashWaitState(DL_SYSCTL_FLASH_WAIT_STATE_2);


    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setHFCLKSourceHFXTParams(DL_SYSCTL_HFXT_RANGE_32_48_MHZ,0, false);
    DL_SYSCTL_configSYSPLL((DL_SYSCTL_SYSPLLConfig *) &gSYSPLLConfig);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_2);
    DL_SYSCTL_enableMFPCLK();
    DL_SYSCTL_setMFPCLKSource(DL_SYSCTL_MFPCLK_SOURCE_SYSOSC);
    DL_SYSCTL_setMCLKSource(SYSOSC, HSCLK, DL_SYSCTL_HSCLK_SOURCE_SYSPLL);

}


static const DL_TimerG_ClockConfig gTIMER_QEIClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 9U
};


SYSCONFIG_WEAK void SYSCFG_DL_TIMER_QEI_init(void) {

    DL_TimerG_setClockConfig(
        TIMER_QEI_INST, (DL_TimerG_ClockConfig *) &gTIMER_QEIClockConfig);

    DL_TimerG_configQEI(TIMER_QEI_INST, DL_TIMER_QEI_MODE_3_INPUT,
        DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_0_INDEX);
    DL_TimerG_configQEI(TIMER_QEI_INST, DL_TIMER_QEI_MODE_3_INPUT,
        DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setLoadValue(TIMER_QEI_INST, 65535);
    DL_TimerG_enableInterrupt(TIMER_QEI_INST , DL_TIMER_EVENT_ZERO_EVENT);

    DL_TimerG_enableClock(TIMER_QEI_INST);
}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (8000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   8000000 Hz = 8000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerA_ClockConfig gTIMER_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_0_INST_LOAD_VALUE = (50us * 8000000 Hz) - 1
 */
static const DL_TimerA_TimerConfig gTIMER_0TimerConfig = {
    .period     = TIMER_0_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_0_init(void) {

    DL_TimerA_setClockConfig(TIMER_0_INST,
        (DL_TimerA_ClockConfig *) &gTIMER_0ClockConfig);

    DL_TimerA_initTimerMode(TIMER_0_INST,
        (DL_TimerA_TimerConfig *) &gTIMER_0TimerConfig);
    DL_TimerA_enableClock(TIMER_0_INST);


    DL_TimerA_enableEvent(TIMER_0_INST, DL_TIMERA_EVENT_ROUTE_1, (DL_TIMERA_EVENT_ZERO_EVENT));

    DL_TimerA_setPublisherChanID(TIMER_0_INST, DL_TIMERA_PUBLISHER_INDEX_0, TIMER_0_INST_PUB_0_CH);



}

/*
 * Timer clock configuration to be sourced by BUSCLK /  (1000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   1000000 Hz = 1000000 Hz / (8 * (0 + 1))
 */
static const DL_TimerA_ClockConfig gTIMER_MSClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 0U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_MS_INST_LOAD_VALUE = (1 ms * 1000000 Hz) - 1
 */
static const DL_TimerA_TimerConfig gTIMER_MSTimerConfig = {
    .period     = TIMER_MS_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC_UP,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_MS_init(void) {

    DL_TimerA_setClockConfig(TIMER_MS_INST,
        (DL_TimerA_ClockConfig *) &gTIMER_MSClockConfig);

    DL_TimerA_initTimerMode(TIMER_MS_INST,
        (DL_TimerA_TimerConfig *) &gTIMER_MSTimerConfig);
    DL_TimerA_enableClock(TIMER_MS_INST);





}


/* ADC12_0 Initialization */
static const DL_ADC12_ClockConfig gADC12_0ClockConfig = {
    .clockSel       = DL_ADC12_CLOCK_SYSOSC,
    .divideRatio    = DL_ADC12_CLOCK_DIVIDE_1,
    .freqRange      = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};
SYSCONFIG_WEAK void SYSCFG_DL_ADC12_0_init(void)
{
    DL_ADC12_setClockConfig(ADC12_0_INST, (DL_ADC12_ClockConfig *) &gADC12_0ClockConfig);
    DL_ADC12_configConversionMem(ADC12_0_INST, ADC12_0_ADCMEM_0,
        DL_ADC12_INPUT_CHAN_2, DL_ADC12_REFERENCE_VOLTAGE_VDDA, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
    DL_ADC12_enableConversions(ADC12_0_INST);
}

static const DL_DMA_Config gDMA_CH0Config = {
    .transferMode   = DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE,
    .extendedMode   = DL_DMA_NORMAL_MODE,
    .destIncrement  = DL_DMA_ADDR_UNCHANGED,
    .srcIncrement   = DL_DMA_ADDR_INCREMENT,
    .destWidth      = DL_DMA_WIDTH_HALF_WORD,
    .srcWidth       = DL_DMA_WIDTH_HALF_WORD,
    .trigger        = DAC12_INST_DMA_TRIGGER,
    .triggerType    = DL_DMA_TRIGGER_TYPE_EXTERNAL,
};

SYSCONFIG_WEAK void SYSCFG_DL_DMA_CH0_init(void)
{
    DL_DMA_initChannel(DMA, DMA_CH0_CHAN_ID , (DL_DMA_Config *) &gDMA_CH0Config);
}
SYSCONFIG_WEAK void SYSCFG_DL_DMA_init(void){
    SYSCFG_DL_DMA_CH0_init();
}


static const DL_DAC12_Config gDAC12Config = {
    .outputEnable              = DL_DAC12_OUTPUT_ENABLED,
    .resolution                = DL_DAC12_RESOLUTION_12BIT,
    .representation            = DL_DAC12_REPRESENTATION_BINARY,
    .voltageReferenceSource    = DL_DAC12_VREF_SOURCE_VDDA_VSSA,
    .amplifierSetting          = DL_DAC12_AMP_ON,
    .fifoEnable                = DL_DAC12_FIFO_ENABLED,
    .fifoTriggerSource         = DL_DAC12_FIFO_TRIGGER_HWTRIG0,
    .dmaTriggerEnable          = DL_DAC12_DMA_TRIGGER_ENABLED,
    .dmaTriggerThreshold       = DL_DAC12_FIFO_THRESHOLD_TWO_QTRS_EMPTY,
    .sampleTimeGeneratorEnable = DL_DAC12_SAMPLETIMER_ENABLE,
    .sampleRate                = DL_DAC12_SAMPLES_PER_SECOND_1M,
};
SYSCONFIG_WEAK void SYSCFG_DL_DAC12_init(void)
{
    DL_DAC12_init(DAC0, (DL_DAC12_Config *) &gDAC12Config);//初始化结构体，寄存器操作即对寄存器类不同元素赋值

    DAC0->CPU_INT.IMASK |= ((uint32_t)0x00004000U);//DL_DAC12_enableInterrupt(DAC0, DL_DAC12_INTERRUPT_DMA_DONE);
    //#define DL_DAC12_INTERRUPT_DMA_DONE                (DAC12_GEN_EVENT_IMASK_DMADONEIFG_SET)
//#define DAC12_GEN_EVENT_IMASK_DMADONEIFG_SET     ((uint32_t)0x00004000U)         /* !< Interrupt will request an interrupt
//                                                                                    service routine and corresponding bit
//                                                                                    in MIS will be set */
    volatile uint32_t *pReg = &DAC0->FSUB_0;
    *(pReg + (uint32_t) DL_DAC12_SUBSCRIBER_INDEX_0) = (1 & ((uint32_t)0x0000000FU));

    //DL_DAC12_setSubscriberChanID(DAC0, DL_DAC12_SUBSCRIBER_INDEX_0, 1);
    //#define DAC12_FSUB_0_CHANID_MAXIMUM              ((uint32_t)0x0000000FU)
    /* !< Consult your device datasheet as
    the actual allowed maximum may be
    less than 15. */
        DAC0->CTL0 |= ((uint32_t)0x00000001U);// DL_DAC12_enable(DAC0); DAC12_CTL0_ENABLE_SET ((uint32_t)0x00000001U)
}

