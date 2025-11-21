#include "ti_msp_dl_config.h"

DL_TimerA_backupConfig gTIMER_5086Backup;
DL_TimerA_backupConfig gTIMER_1SBackup;

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
    SYSCFG_DL_TIMER_5086_init();
    SYSCFG_DL_TIMER_1S_init();
    SYSCFG_DL_ADC12_0_init();
    SYSCFG_DL_ADC12_1_init();
    /* Ensure backup structures have no valid state */
    gTIMER_5086Backup.backupRdy     = false;
    gTIMER_1SBackup.backupRdy   = false;

}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

    retStatus &= DL_TimerA_saveConfiguration(TIMER_5086_INST, &gTIMER_5086Backup);
    retStatus &= DL_TimerA_saveConfiguration(TIMER_1S_INST, &gTIMER_1SBackup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

    retStatus &= DL_TimerA_restoreConfiguration(TIMER_5086_INST, &gTIMER_5086Backup, false);
    retStatus &= DL_TimerA_restoreConfiguration(TIMER_1S_INST, &gTIMER_1SBackup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(TIMER_5086_INST);
    DL_TimerA_reset(TIMER_1S_INST);
    DL_ADC12_reset(ADC12_0_INST);
    DL_ADC12_reset(ADC12_1_INST);
    DL_MathACL_reset(MATHACL);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(TIMER_5086_INST);
    DL_TimerA_enablePower(TIMER_1S_INST);
    DL_ADC12_enablePower(ADC12_0_INST);
    DL_ADC12_enablePower(ADC12_1_INST);
    DL_MathACL_enablePower(MATHACL);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initDigitalOutput(GPIO_LED1_LED1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_OLED_13_GPIO_SDA_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_OLED_13_GPIO_SCK_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDs_LED_R_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDs_LED_G_IOMUX);

    DL_GPIO_clearPins(GPIOA, GPIO_LED1_LED1_PIN);
    DL_GPIO_setPins(GPIOA, GPIO_OLED_13_GPIO_SDA_PIN |
        GPIO_OLED_13_GPIO_SCK_PIN);
    DL_GPIO_enableOutput(GPIOA, GPIO_LED1_LED1_PIN |
        GPIO_OLED_13_GPIO_SDA_PIN |
        GPIO_OLED_13_GPIO_SCK_PIN);
    DL_GPIO_clearPins(GPIO_LEDs_PORT, GPIO_LEDs_LED_R_PIN |
        GPIO_LEDs_LED_G_PIN);
    DL_GPIO_enableOutput(GPIO_LEDs_PORT, GPIO_LEDs_LED_R_PIN |
        GPIO_LEDs_LED_G_PIN);

}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   2000000 Hz = 4000000 Hz / (8 * (1 + 1))
 */
static const DL_TimerA_ClockConfig gTIMER_5086ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 1U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_5086_INST_LOAD_VALUE = (0.000196618s * 2000000 Hz) - 1
 */
static const DL_TimerA_TimerConfig gTIMER_5086TimerConfig = {
    .period     = TIMER_5086_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_5086_init(void) {

    DL_TimerA_setClockConfig(TIMER_5086_INST,
        (DL_TimerA_ClockConfig *) &gTIMER_5086ClockConfig);

    DL_TimerA_initTimerMode(TIMER_5086_INST,
        (DL_TimerA_TimerConfig *) &gTIMER_5086TimerConfig);
    DL_TimerA_enableInterrupt(TIMER_5086_INST , DL_TIMERA_INTERRUPT_ZERO_EVENT);
    DL_TimerA_enableClock(TIMER_5086_INST);





}

/*
 * Timer clock configuration to be sourced by BUSCLK /  (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   40000 Hz = 4000000 Hz / (8 * (99 + 1))
 */
static const DL_TimerA_ClockConfig gTIMER_1SClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 99U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_1S_INST_LOAD_VALUE = (1 s * 40000 Hz) - 1
 */
static const DL_TimerA_TimerConfig gTIMER_1STimerConfig = {
    .period     = TIMER_1S_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC_UP,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_1S_init(void) {

    DL_TimerA_setClockConfig(TIMER_1S_INST,
        (DL_TimerA_ClockConfig *) &gTIMER_1SClockConfig);

    DL_TimerA_initTimerMode(TIMER_1S_INST,
        (DL_TimerA_TimerConfig *) &gTIMER_1STimerConfig);
    DL_TimerA_enableInterrupt(TIMER_1S_INST , DL_TIMERA_INTERRUPT_ZERO_EVENT);
    DL_TimerA_enableClock(TIMER_1S_INST);





}


/* ADC12_0 Initialization */
static const DL_ADC12_ClockConfig gADC12_0ClockConfig = {
    .clockSel       = DL_ADC12_CLOCK_SYSOSC,
    .divideRatio    = DL_ADC12_CLOCK_DIVIDE_8,
    .freqRange      = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};
SYSCONFIG_WEAK void SYSCFG_DL_ADC12_0_init(void)
{
    DL_ADC12_setClockConfig(ADC12_0_INST, (DL_ADC12_ClockConfig *) &gADC12_0ClockConfig);
    DL_ADC12_initSingleSample(ADC12_0_INST,
        DL_ADC12_REPEAT_MODE_ENABLED, DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_SOFTWARE,
        DL_ADC12_SAMP_CONV_RES_12_BIT, DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);
    DL_ADC12_configConversionMem(ADC12_0_INST, ADC12_0_ADCMEM_0,
        DL_ADC12_INPUT_CHAN_11, DL_ADC12_REFERENCE_VOLTAGE_VDDA, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_TRIGGER_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
    DL_ADC12_setSampleTime0(ADC12_0_INST,200);
    /* Enable ADC12 interrupt */
    DL_ADC12_clearInterruptStatus(ADC12_0_INST,(DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED));
    DL_ADC12_enableInterrupt(ADC12_0_INST,(DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED));
    DL_ADC12_enableConversions(ADC12_0_INST);
}
/* ADC12_1 Initialization */
static const DL_ADC12_ClockConfig gADC12_1ClockConfig = {
    .clockSel       = DL_ADC12_CLOCK_SYSOSC,
    .divideRatio    = DL_ADC12_CLOCK_DIVIDE_32,
    .freqRange      = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};
SYSCONFIG_WEAK void SYSCFG_DL_ADC12_1_init(void)
{
    DL_ADC12_setClockConfig(ADC12_1_INST, (DL_ADC12_ClockConfig *) &gADC12_1ClockConfig);
    DL_ADC12_initSingleSample(ADC12_1_INST,
        DL_ADC12_REPEAT_MODE_ENABLED, DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_SOFTWARE,
        DL_ADC12_SAMP_CONV_RES_12_BIT, DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);
    DL_ADC12_setStartAddress(ADC12_1_INST, DL_ADC12_SEQ_START_ADDR_01);
    DL_ADC12_configConversionMem(ADC12_1_INST, ADC12_1_ADCMEM_0,
        DL_ADC12_INPUT_CHAN_3, DL_ADC12_REFERENCE_VOLTAGE_VDDA, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_TRIGGER_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
    DL_ADC12_setPowerDownMode(ADC12_1_INST,DL_ADC12_POWER_DOWN_MODE_MANUAL);
    DL_ADC12_setSampleTime0(ADC12_1_INST,60000);
    DL_ADC12_setSampleTime1(ADC12_1_INST,0);
    /* Enable ADC12 interrupt */
    DL_ADC12_clearInterruptStatus(ADC12_1_INST,(DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED));
    DL_ADC12_enableInterrupt(ADC12_1_INST,(DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED));
    DL_ADC12_enableConversions(ADC12_1_INST);
}

