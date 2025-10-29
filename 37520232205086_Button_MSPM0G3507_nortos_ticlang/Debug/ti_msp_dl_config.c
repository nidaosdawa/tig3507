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

DL_TimerA_backupConfig gLED_rgbBackup;
DL_TimerG_backupConfig gTIMER_QEIBackup;
DL_TimerG_backupConfig gTIMER_MSBackup;

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
    SYSCFG_DL_LED_rgb_init();
    SYSCFG_DL_TIMER_QEI_init();
    SYSCFG_DL_TIMER_MS_init();
    /* Ensure backup structures have no valid state */
	gLED_rgbBackup.backupRdy 	= false;
	gTIMER_QEIBackup.backupRdy 	= false;
	gTIMER_MSBackup.backupRdy 	= false;

}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(LED_rgb_INST, &gLED_rgbBackup);
	retStatus &= DL_TimerG_saveConfiguration(TIMER_QEI_INST, &gTIMER_QEIBackup);
	retStatus &= DL_TimerG_saveConfiguration(TIMER_MS_INST, &gTIMER_MSBackup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(LED_rgb_INST, &gLED_rgbBackup, false);
	retStatus &= DL_TimerG_restoreConfiguration(TIMER_QEI_INST, &gTIMER_QEIBackup, false);
	retStatus &= DL_TimerG_restoreConfiguration(TIMER_MS_INST, &gTIMER_MSBackup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(LED_rgb_INST);
    DL_TimerG_reset(TIMER_QEI_INST);
    DL_TimerG_reset(TIMER_MS_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(LED_rgb_INST);
    DL_TimerG_enablePower(TIMER_QEI_INST);
    DL_TimerG_enablePower(TIMER_MS_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(GPIO_LED_rgb_C0_IOMUX,GPIO_LED_rgb_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_LED_rgb_C0_PORT, GPIO_LED_rgb_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_LED_rgb_C1_IOMUX,GPIO_LED_rgb_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_LED_rgb_C1_PORT, GPIO_LED_rgb_C1_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_LED_rgb_C2_IOMUX,GPIO_LED_rgb_C2_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_LED_rgb_C2_PORT, GPIO_LED_rgb_C2_PIN);

    DL_GPIO_initPeripheralInputFunction(GPIO_TIMER_QEI_PHA_IOMUX,GPIO_TIMER_QEI_PHA_IOMUX_FUNC);
    DL_GPIO_initPeripheralInputFunction(GPIO_TIMER_QEI_PHB_IOMUX,GPIO_TIMER_QEI_PHB_IOMUX_FUNC);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_LED_1_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GPIO_EC11_SW_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalOutput(GPIO_OLED_13_GPIO_SCK_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_OLED_13_GPIO_SDA_IOMUX);

    DL_GPIO_setPins(GPIOA, GPIO_LEDS_LED_1_PIN |
		GPIO_OLED_13_GPIO_SCK_PIN |
		GPIO_OLED_13_GPIO_SDA_PIN);
    DL_GPIO_enableOutput(GPIOA, GPIO_LEDS_LED_1_PIN |
		GPIO_OLED_13_GPIO_SCK_PIN |
		GPIO_OLED_13_GPIO_SDA_PIN);
    DL_GPIO_setUpperPinsPolarity(GPIOA, DL_GPIO_PIN_18_EDGE_RISE_FALL);
    DL_GPIO_clearInterruptStatus(GPIOA, GPIO_EC11_SW_PIN);
    DL_GPIO_enableInterrupt(GPIOA, GPIO_EC11_SW_PIN);

}



SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    
	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
	/* Set default configuration */
	DL_SYSCTL_disableHFXT();
	DL_SYSCTL_disableSYSPLL();

}


/*
 * Timer clock configuration to be sourced by  / 2 (16000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   16000000 Hz = 16000000 Hz / (2 * (0 + 1))
 */
static const DL_TimerA_ClockConfig gLED_rgbClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_2,
    .prescale = 0U
};

static const DL_TimerA_PWMConfig gLED_rgbConfig = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .isTimerWithFourCC = true,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_LED_rgb_init(void) {

    DL_TimerA_setClockConfig(
        LED_rgb_INST, (DL_TimerA_ClockConfig *) &gLED_rgbClockConfig);

    DL_TimerA_initPWMMode(
        LED_rgb_INST, (DL_TimerA_PWMConfig *) &gLED_rgbConfig);

    // Set Counter control to the smallest CC index being used
    DL_TimerA_setCounterControl(LED_rgb_INST,DL_TIMER_CZC_CCCTL0_ZCOND,DL_TIMER_CAC_CCCTL0_ACOND,DL_TIMER_CLC_CCCTL0_LCOND);

    DL_TimerA_setCaptureCompareOutCtl(LED_rgb_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_ENABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_0_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(LED_rgb_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_0_INDEX);
    DL_TimerA_setCaptureCompareValue(LED_rgb_INST, 500, DL_TIMER_CC_0_INDEX);

    DL_TimerA_setCaptureCompareOutCtl(LED_rgb_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_ENABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_1_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(LED_rgb_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_1_INDEX);
    DL_TimerA_setCaptureCompareValue(LED_rgb_INST, 1000, DL_TIMER_CC_1_INDEX);

    DL_TimerA_setCaptureCompareOutCtl(LED_rgb_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_ENABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_2_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(LED_rgb_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_2_INDEX);
    DL_TimerA_setCaptureCompareValue(LED_rgb_INST, 1000, DL_TIMER_CC_2_INDEX);

    DL_TimerA_enableClock(LED_rgb_INST);


    
    DL_TimerA_setCCPDirection(LED_rgb_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT | DL_TIMER_CC2_OUTPUT );


}


static const DL_TimerG_ClockConfig gTIMER_QEIClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};


SYSCONFIG_WEAK void SYSCFG_DL_TIMER_QEI_init(void) {

    DL_TimerG_setClockConfig(
        TIMER_QEI_INST, (DL_TimerG_ClockConfig *) &gTIMER_QEIClockConfig);

    DL_TimerG_configQEI(TIMER_QEI_INST, DL_TIMER_QEI_MODE_2_INPUT,
        DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_0_INDEX);
    DL_TimerG_configQEI(TIMER_QEI_INST, DL_TIMER_QEI_MODE_2_INPUT,
        DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setLoadValue(TIMER_QEI_INST, 65535);
    DL_TimerG_enableClock(TIMER_QEI_INST);
}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   1000000 Hz = 32000000 Hz / (1 * (31 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_MSClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 31U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_MS_INST_LOAD_VALUE = (1 ms * 1000000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_MSTimerConfig = {
    .period     = TIMER_MS_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC_UP,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_MS_init(void) {

    DL_TimerG_setClockConfig(TIMER_MS_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_MSClockConfig);

    DL_TimerG_initTimerMode(TIMER_MS_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_MSTimerConfig);
    DL_TimerG_enableInterrupt(TIMER_MS_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMER_MS_INST);





}


