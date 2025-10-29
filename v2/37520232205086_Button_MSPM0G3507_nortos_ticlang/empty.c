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

// QEI��ر���
volatile uint32_t lastQEIValue = 0;
volatile uint32_t lastQEITime = 0;
volatile uint32_t qeiPeriod = 0;      // ��ת����(ms)
volatile int32_t  qeiDirection = 0;   // ��ת����: 1=˳ʱ��, -1=��ʱ��
volatile float    qeiPhaseLag_ms = 0; // �ͺ���λ(ms)��������ʾ�ͺ󣬸�����ʾ��ǰ

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
/* �����ж� */
void GPIO_EC11_INT_IRQHandler(void)
{
    DL_GPIO_clearInterruptStatus(GPIOA, GPIO_EC11_SW_IIDX );
    uint8_t level = DL_GPIO_readPins(GPIO_EC11_PORT, GPIO_EC11_SW_PIN);


    if (level == 1) {  // ���£��ߵ�ƽ��Ч��
        keyPressed = 1;
        keyPressTime = g_ms;
        keyLongDone = 0;
        keyRising = 0;
    } else {           // �ͷ�
        keyPressed = 0;
        if (!keyLongDone) keyRising = 1;
    }
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

    while (1) {
        /* ---------- QEI�������� ---------- */
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

        if (snap_cnt != 0) {
            if (!inSetMode) {
                /* ��������ʾ���������ڡ��ͺ���λ�����룩 */
                char line[64];
                OLED_Clear();

                // ��0�У���ʾ����
                OLED_ShowString(0, 0, snap_dir==1?"Clockwise" : "Anticlockwise");

                // ��2�У���ʾ����
                if (snap_period > 0) {
                    snprintf(line, sizeof(line), "Period: %lu ms", snap_period);
                } else {
                    snprintf(line, sizeof(line), "Period: -- ms");
                }
                OLED_ShowString(0, 2, line);

                // ��4�У���ʾ��λ����룩
                snprintf(line, sizeof(line), "Phase: %+.1f ms", snap_phase_ms);
                               OLED_ShowString(0, 4, line);


                // ��6�У���ʾ��������ѡ��
                snprintf(line, sizeof(line), "Count: %+ld", (long)snap_cnt);
                OLED_ShowString(0, 6, line);

                OLED_Refresh();
            } else {
                /* ����ģʽ������ */
                int delta = (snap_dir > 0) ? 10 : -10;
                uint8_t *p = (colorIdx == 0) ? &rgb.r :
                            (colorIdx == 1) ? &rgb.g : &rgb.b;
                int tmp = (int)(*p) + delta * abs(snap_cnt);
                if (tmp > 100) tmp = 100;
                if (tmp < 0) tmp = 0;
                *p = (uint8_t)tmp;
                rgbUpdate();

                // ˢ�����ý���
                OLED_Clear();
                char line[32];
                snprintf(line, sizeof(line), "SET %c", "RGB"[colorIdx]);
                OLED_ShowString(0, 0, line);
                snprintf(line, sizeof(line), "Value: %d%%", *p);
                OLED_ShowString(0, 2, line);
                OLED_Refresh();
            }
        }

        /* ---------- �������� ---------- */
        if (keyRising) {
            keyRising = 0;
            if (!inSetMode) {
                enterSetMode();  // �̰���������
            } else {
                colorIdx = (colorIdx + 1) % 3;
                // ˢ����ɫͨ����ʾ
                OLED_Clear();
                char line[32];
                snprintf(line, sizeof(line), "SET %c", "RGB"[colorIdx]);
                OLED_ShowString(0, 0, line);
                snprintf(line, sizeof(line), "Value: %d%%",
                        colorIdx == 0 ? rgb.r : colorIdx == 1 ? rgb.g : rgb.b);
                OLED_ShowString(0, 2, line);
                OLED_Refresh();
                lastRotTime = g_ms;  // ���ó�ʱ��ʱ��
            }
        }

        /* ---------- ������� ---------- */
        if (!inSetMode && keyPressed && !keyLongDone) {
            if (g_ms - keyPressTime >= 3000) {  // 3�볤��
                keyLongDone = 1;
                enterSetMode();
            }
        }

        /* ---------- ����ģʽ��ʱ�˳� ---------- */
        if (inSetMode && (g_ms - lastRotTime) >= 5000) {
            exitSetMode(0);  // 5���޲���ȡ��
        }

        // �����ӳ٣�����CPUʹ����
        delay_cycles(1000);
    }
}
