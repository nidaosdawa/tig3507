/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
// ==========================================================================
//  OLEDģ��PCB˿ӡ     ΢���������ߣ����ţ�
//  VCC                 �� 3.3v ��Դ
//  GND                 ��Դ��
//  SCK                 PA31 ��ʱ�ӣ�
//  SDA                 PA28 �����ݣ�

// ============================================================================
// main ������ÿ��������Ŀ������ֻ��1��

// ==========================================================================
//  ������ͷ�ļ�
// ==========================================================================
/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY IMPLIED WARRANTIES ARE DISCLAIMED.
 */

#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "I2C_GPIO_25a.h"
#include "OLED13.h"
#include "OLED_BMP.h"       // ͼƬ������ʾͼƬʱ���ɲ��������ļ�

#define DELAY (16000000)  // 0.5 s @ 32 MHz
#define BEEP_SHORT_US       (50000UL)      // 50 ms
#define BEEP_LONG_US        (200000UL)     // 200 ms
#define SET_TIMEOUT_MS      (5000U)        // 5 s �޲����Զ��˳�
/* ---------- ȫ�ֱ��� ---------- */
volatile uint32_t g_ms = 0;
volatile int32_t  Ex_cnt = 0;
volatile uint8_t  keyPressed = 0;
volatile uint8_t  keyRising = 0;
volatile uint8_t  keyLongDone = 0;
volatile uint32_t keyPressTime = 0;

typedef struct { uint8_t r, g, b; } RGB_t;
static RGB_t rgb = {50, 0, 0};
static uint8_t colorIdx = 0;
static uint8_t inSetMode = 0;
static uint32_t lastRotTime = 0;
/* ����״̬�� */
typedef enum { SET_NEXT_COLOR, SET_EXIT } SetStep_t;
static SetStep_t setStep = SET_NEXT_COLOR;

// QEI��ر���
volatile uint32_t lastQEIValue = 0;
volatile uint32_t lastQEITime = 0;
volatile uint32_t qeiPeriod = 0;      // ��ת����(ms)
volatile int32_t  qeiDirection = 0;   // ��ת����: 1=˳ʱ��, -1=��ʱ��
volatile float    qeiPhaseLag_ms = 0; // �ͺ���λ(ms)��������ʾ�ͺ󣬸�����ʾ��ǰ

// ����״̬������
static uint8_t lastKeyState = 0;  // �����ʼΪ�ͷ�״̬���͵�ƽ��
static uint32_t keyDebounceTime = 0;
static uint8_t keyStableState = 1;

/* ---------- ����ԭ�� ---------- */
static void rgbUpdate(void);
static void enterSetMode(void);
static void exitSetMode(uint8_t save);
static void updateQEIParams(void);

static float calculatePhaseLag_ms(uint32_t period);

/* ========================================================================== */
/* 1 ms ϵͳ���� */
void TIMER_MS_INST_IRQHandler(void)
{
    g_ms++;
    DL_TimerG_clearInterruptStatus(TIMER_MS_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);
}

/* ========================================================================== */

/* ========================================================================== */
void GROUP1_IRQHandler(void)
{
    /* 1. �����־���ٶ���ƽ */
    if (DL_GPIO_getPendingInterrupt(GPIOA) & GPIO_EC11_SW_IIDX)
    {
        DL_GPIO_clearInterruptStatus(GPIOA, GPIO_EC11_SW_IIDX);

        uint8_t level = DL_GPIO_readPins(GPIO_EC11_PORT, GPIO_EC11_SW_PIN);

        if (level) {               // �ߵ�ƽ=����
            keyPressed   = 1;
            keyPressTime = g_ms;
            keyLongDone  = 0;
            keyRising    = 0;
        } else {                   // �͵�ƽ=�ͷ�
            keyPressed = 0;
            if (!keyLongDone) keyRising = 1;
        }
    }
}
/* ---------- ������ ---------- */
/* ���������ߵ�ƽ�죬us ����ʱ�� */
static void beep(uint32_t us)
{
    DL_GPIO_setPins(GPIO_PORT, GPIO_BUZZER_PIN);
    delay_cycles(us * 32UL);          // 32 MHz �� 1 us �� 32 cycles
    DL_GPIO_clearPins(GPIO_PORT, GPIO_BUZZER_PIN);
}
/* ========================================================================== */
/* �����ͺ���λ�����룩 */
static float calculatePhaseLag_ms(uint32_t period)
{
    if (period == 0) return 0.0f;

    // EC11��A��B����90����λ��
    // ��90����λ��ת��Ϊʱ�䣨���룩
    // 90�� = 1/4����
    float phase_time_ms = (float)period / 4.0f;

    // ������ת����ȷ����λ��ϵ��
    // ˳ʱ�룺B���ͺ�A�� �� ����λ��
    // ��ʱ�룺B�೬ǰA�� �� ����λ��
    return (qeiDirection > 0) ? phase_time_ms : -phase_time_ms;
}

/* ========================================================================== */
/* ����QEI���� */
static void updateQEIParams(void)
{
    uint32_t currentQEI = DL_TimerG_getTimerCount(TIMER_QEI_INST);
    uint32_t currentTime = g_ms;

    // �������仯
    if (currentQEI != lastQEIValue) {
        // �жϷ���
        if ((int32_t)(currentQEI - lastQEIValue) > 0) {
            qeiDirection = 1;  // ˳ʱ��
            Ex_cnt += (currentQEI - lastQEIValue);
        } else {
            qeiDirection = -1; // ��ʱ��
            Ex_cnt -= (lastQEIValue - currentQEI);
        }

        // ������ת���ڣ�����ʱ�����
        if (lastQEITime != 0) {
            qeiPeriod = currentTime - lastQEITime;

            // �����ͺ���λ�����룩
            qeiPhaseLag_ms = calculatePhaseLag_ms(qeiPeriod);
        }

        lastQEIValue = currentQEI;
        lastQEITime = currentTime;
        lastRotTime = currentTime;
    }
}

/* ========================================================================== */
static void rgbUpdate(void)
{
    DL_TimerA_setCaptureCompareValue(LED_rgb_INST, rgb.r * 10, DL_TIMERA_CAPTURE_COMPARE_0_INDEX);
    DL_TimerA_setCaptureCompareValue(LED_rgb_INST, rgb.g * 10, DL_TIMERA_CAPTURE_COMPARE_1_INDEX);
    DL_TimerA_setCaptureCompareValue(LED_rgb_INST, rgb.b * 10, DL_TIMERA_CAPTURE_COMPARE_2_INDEX);
}
/*========================================================================== */
/* ��ʾ��ǰ��ɫ���ý��� */
static void showColorSet(void)
{
    OLED_Clear();
    char line[16];
    snprintf(line, sizeof(line), "SET %c", "RGB"[colorIdx]);
    OLED_ShowString(0, 0, line);
    uint8_t *p = (colorIdx == 0) ? &rgb.r :
                 (colorIdx == 1) ? &rgb.g : &rgb.b;
    snprintf(line, sizeof(line), "Value: %d%%", *p);
    OLED_ShowString(0, 2, line);
    OLED_Refresh();
}

static void enterSetMode(void)
{
    inSetMode = 1;
    colorIdx = 0;
    lastRotTime = g_ms;
    OLED_Clear();
    OLED_ShowString(0, 0, "SET R");
    char line[16];
    snprintf(line, sizeof(line), "Value: %d%%", rgb.r);
    OLED_ShowString(0, 2, line);

    rgbUpdate();
    OLED_Refresh();
}

static void exitSetMode(uint8_t save)
{
    inSetMode = 0;
    if (!save) {
        rgb = (RGB_t){50, 0, 0};
        rgbUpdate();
    }
    OLED_Clear();
    OLED_ShowString(0, 0, save ? "Saved!" : "Canceled!");
    OLED_Refresh();
    delay_cycles(500000);
    OLED_Clear();
    OLED_ShowString(0, 0, "EC11 QEI Meter");
    OLED_Refresh();
}

/* ========================================================================== */
int main(void)
{
    SYSCFG_DL_init();

   DL_GPIO_disableInterrupt(GPIOA, GPIO_OLED_13_GPIO_SCK_PIN |
                                   GPIO_OLED_13_GPIO_SDA_PIN);
    NVIC_EnableIRQ(TIMER_MS_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_QEI_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_EC11_INT_IRQN);

    DL_TimerG_startCounter(TIMER_MS_INST);
    DL_TimerG_startCounter(TIMER_QEI_INST);

    I2CB1_Init();
    OLED_Init();
    // ��ʼ����ʾ
    OLED_Clear();
   OLED_ShowCHinese(0, 0, 0);
   OLED_ShowCHinese(16 * 1, 0, 1);
   OLED_ShowCHinese(16 * 2, 0, 2);
   OLED_ShowString(0, 2, "YuntingDong");
   OLED_ShowString(0, 4, "5086");
   delay_cycles(DELAY);
   OLED_Clear();
    OLED_ShowString(0, 0, "EC11 QEI Meter");
    OLED_Refresh();

    // ��ʼ��QEI����
        lastQEIValue = DL_TimerG_getTimerCount(TIMER_QEI_INST);
        lastQEITime = g_ms;

        // ��ʼ������״̬
        lastKeyState = DL_GPIO_readPins(GPIO_EC11_PORT, GPIO_EC11_SW_PIN);
        keyStableState = lastKeyState;

    while (1) {
//        /* ---------- QEI�������� ---------- */
        updateQEIParams();


        /* ---------- ��ת���� ---------- */
        int32_t snap_cnt = 0;
        uint32_t snap_period = 0;
        int32_t snap_dir = 0;
        float snap_phase_ms = 0;

        __disable_irq();
        if (Ex_cnt != 0) {
            snap_cnt = Ex_cnt;
            snap_period = qeiPeriod;
            snap_dir = qeiDirection;
            snap_phase_ms = qeiPhaseLag_ms;
            Ex_cnt = 0;  // ���ü���
        }
        __enable_irq();
        if (snap_cnt != 0 && !inSetMode) {
                    char line[64];
                    OLED_Clear();
                    OLED_ShowString(0, 0, snap_dir == 1 ? "Clockwise" : "Anticlockwise");
                    snprintf(line, sizeof(line), "Period: %lu ms", snap_period);
                    OLED_ShowString(0, 2, line);
                    snprintf(line, sizeof(line), "Phase: %+.1f ms", snap_phase_ms);
                    OLED_ShowString(0, 4, line);
                    snprintf(line, sizeof(line), "Count: %+ld", (long)snap_cnt);
                    OLED_ShowString(0, 6, line);
                    OLED_Refresh();
                }

                /* ����ģʽ�ڣ���ת������ */
                if (snap_cnt != 0 && inSetMode) {
                    int delta = (snap_dir > 0) ? 10 : -10;
                    uint8_t *p = (colorIdx == 0) ? &rgb.r :
                                 (colorIdx == 1) ? &rgb.g : &rgb.b;
                    int tmp = (int)*p + delta * abs(snap_cnt);
                    if (tmp > 100) tmp = 100;
                    if (tmp < 0)   tmp = 0;
                    *p = (uint8_t)tmp;
                    rgbUpdate();
                    showColorSet();
                    lastRotTime = g_ms;
                }

                /* ---------- �������� ---------- */
                if (keyRising) {
                    keyRising = 0;
                    beep(BEEP_SHORT_US);          // ��������

                    if (!inSetMode) {             // ��һ�ν���
                        enterSetMode();
                        beep(BEEP_LONG_US);
                    } else {                      // ����������
                        switch (setStep) {
                        case SET_NEXT_COLOR:
                            colorIdx = (colorIdx + 1) % 3;
                            lastRotTime = g_ms;
                            showColorSet();
                            if (colorIdx == 0) {  // �ص� R �� ��һ���˳�
                                setStep = SET_EXIT;
                            }
                            break;
                        case SET_EXIT:
                            exitSetMode(1);       // ���沢�˳�
                            beep(BEEP_LONG_US);
                            break;
                        }
                    }
                }

                /* 5 ������ת�Զ��˳��������޸ģ� */
                if (inSetMode && (g_ms - lastRotTime) >= SET_TIMEOUT_MS) {
                    exitSetMode(0);               // ������
                    beep(BEEP_LONG_US);
                }

                delay_cycles(1000);
            }
        }
