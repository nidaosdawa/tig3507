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
//  OLED模块PCB丝印     微控制器连线（引脚）
//  VCC                 接 3.3v 电源
//  GND                 电源地
//  SCK                 PA31 （时钟）
//  SDA                 PA28 （数据）

// ============================================================================
// main 函数，每个工程项目内有且只有1个

// ==========================================================================
//  处理器头文件
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
#include "string.h"
#include "I2C_GPIO_25a.h"
#include "OLED13.h"

#define DELAY (16000000)  // 0.5 s @ 32 MHz

/* ---------- 全局变量 ---------- */
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
//QEI相关变量
// QEI相关变量
volatile uint32_t lastQEIValue = 0;
volatile uint32_t lastQEITime = 0;
volatile uint32_t qeiPeriod = 0;      // 旋转周期(ms)
volatile int32_t  qeiDirection = 0;   // 旋转方向: 1=顺时针, -1=逆时针

/* ---------- 函数原型 ---------- */
static void rgbUpdate(void);
static void enterSetMode(void);
static void exitSetMode(uint8_t save);
static void updateQEIParams(void);

/* ========================================================================== */
/* 1 ms 系统节拍 */
void TIMER_MS_INST_IRQHandler(void)
{
    g_ms++;
    DL_TimerG_clearInterruptStatus(TIMER_MS_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);
}

/* ========================================================================== */
void TIMER_QEI_INST_IRQHandler(void)
{
    // 可以在这里处理QEI的特殊事件，如索引脉冲
    DL_TimerG_clearInterruptStatus(TIMER_QEI_INST, DL_TIMER_INTERRUPT_OVERFLOW);
}
/* ========================================================================== */
/* 按键中断 */
void GPIO_EC11_GPIOA_INT_IRQHandler(void)
{
    DL_GPIO_clearInterruptStatus(GPIOA, GPIO_EC11_GPIOA_INT_IIDX);
    uint8_t level = DL_GPIO_readPins(GPIO_EC11_SW_PORT, GPIO_EC11_SW_PIN);

    if (level == 0) {  // 按下（假设低电平有效）
        keyPressed = 1;
        keyPressTime = g_ms;
        keyLongDone = 0;
        keyRising = 0;
    } else {           // 释放
        keyPressed = 0;
        if (!keyLongDone) keyRising = 1;
    }
}
/* ========================================================================== */
/* 更新QEI参数 - 在主循环中调用 */
static void updateQEIParams(void)
{
    uint32_t currentQEI = DL_TimerG_getCount(TIMER_QEI_INST);
    uint32_t currentTime = g_ms;

    // 检测计数变化
    if (currentQEI != lastQEIValue) {
        // 判断方向：当前值 > 上次值 为顺时针，否则逆时针
        if ((int32_t)(currentQEI - lastQEIValue) > 0) {
            qeiDirection = 1;  // 顺时针
            Ex_cnt += (currentQEI - lastQEIValue);
        } else {
            qeiDirection = -1; // 逆时针
            Ex_cnt -= (lastQEIValue - currentQEI);
        }

        // 计算旋转周期
        if (lastQEITime != 0) {
            qeiPeriod = currentTime - lastQEITime;
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
    rgbUpdate();
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
    OLED_ShowString(0, 0, "EC11 Meter");
    OLED_Refresh();
}

/* ========================================================================== */
int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(TIMER_MS_INST_INT_IRQN);

    DL_TimerG_setQeiCountMode(TIMER_QEI_INST, DL_TIMER_QEI_COUNT_BOTH_EDGES);
    DL_TimerG_setQeiCountDirection(TIMER_QEI_INST, DL_TIMER_QEI_DIR_NORMAL);
    DL_TimerG_startCounter(TIMER_QEI_INST);
    DL_TimerG_startCounter(TIMER_MS_INST);
    lastQEIValue = DL_TimerG_getCount(TIMER_QEI_INST);
        lastQEITime = g_ms;
    I2CB1_Init();
    OLED_Init();

    // 初始化显示
    OLED_Clear();
    OLED_ShowCHinese(0, 0, 0);
    OLED_ShowCHinese(16 * 1, 0, 1);
    OLED_ShowCHinese(16 * 2, 0, 2);
    OLED_ShowString(0, 2, "YuntingDong");
    OLED_ShowString(0, 4, "5086");
        delay_cycles(DELAY);
        OLED_Clear();
    OLED_ShowString(0, 0, "EC11 Meter");

    delay_cycles(DELAY);
    OLED_Refresh();

    while (1) {
        updateQEIParams();

        /* ---------- 旋转处理 ---------- */
               int32_t snap_cnt = 0;
               uint32_t snap_period = 0;
               int32_t snap_dir = 0;

        __disable_irq();
        if (Ex_cnt != 0) {
                    snap_cnt = Ex_cnt;
                    snap_period = qeiPeriod;
                    snap_dir = qeiDirection;
                    Ex_cnt = 0;  // 重置计数
                }
        __enable_irq();

        if (snap_cnt != 0) {
            if (!inSetMode) {
                /* 主界面显示 */
                uint8_t dir = (snap_cnt > 0);
                float taMs = (float)snap_ta;
                float dtMs = (float)snap_dt;

                char line[64];
                OLED_Clear();
                OLED_ShowString(0, 0, dir ? "Clockwise" : "Anticlockwise");
                snprintf(line, sizeof(line), "TA=%lu ms", snap_ta);
                OLED_ShowString(0, 2, line);
                snprintf(line, sizeof(line), "Dt=%+ld ms", (long)snap_dt);
                OLED_ShowString(0, 4, line);
                snprintf(line, sizeof(line), "Cnt=%+ld", (long)snap_cnt);
                OLED_ShowString(0, 6, line);

                OLED_Refresh();
            } else {
                /* 设置模式调亮度 */
                int delta = (snap_cnt > 0) ? 10 : -10;
                uint8_t *p = (colorIdx == 0) ? &rgb.r :
                            (colorIdx == 1) ? &rgb.g : &rgb.b;
                int tmp = (int)(*p) + delta;
                if (tmp > 100) tmp = 100;
                if (tmp < 0) tmp = 0;
                *p = (uint8_t)tmp;
                rgbUpdate();

                // 刷新设置界面
                OLED_Clear();
                char line[32];
                snprintf(line, sizeof(line), "SET %c", "RGB"[colorIdx]);
                OLED_ShowString(0, 0, line);
                snprintf(line, sizeof(line), "Value: %d%%", *p);
                OLED_ShowString(0, 2, line);
                OLED_Refresh();
            }
        }

        /* ---------- 按键处理 ---------- */
        if (keyRising) {
            keyRising = 0;
            if (!inSetMode) {
                enterSetMode();  // 短按进入设置
            } else {
                colorIdx = (colorIdx + 1) % 3;
                if (colorIdx == 0) {
                    exitSetMode(1);  // 循环完RGB后保存退出
                } else {
                    // 刷新颜色通道显示
                    OLED_Clear();
                    char line[32];
                    snprintf(line, sizeof(line), "SET %c", "RGB"[colorIdx]);
                    OLED_ShowString(0, 0, line);
                    snprintf(line, sizeof(line), "Value: %d%%",
                            colorIdx == 0 ? rgb.r : colorIdx == 1 ? rgb.g : rgb.b);
                    OLED_ShowString(0, 2, line);
                    OLED_Refresh();
                }
            }
        }

        /* ---------- 长按检测 ---------- */
        if (!inSetMode && keyPressed && !keyLongDone) {
            if (g_ms - keyPressTime >= 3000) {  // 3秒长按
                keyLongDone = 1;
                enterSetMode();
            }
        }

        /* ---------- 设置模式超时退出 ---------- */
        if (inSetMode && (g_ms - lastRotTime) >= 5000) {
            exitSetMode(0);  // 5秒无操作取消
        }
    }
}
