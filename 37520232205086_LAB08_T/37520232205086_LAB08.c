/*
 * EC11 可调三角波发生器  –  频率 / 幅值 / 占空比  三可调
 * 主频 4 MHz，TimerA0 触发 DAC0，DMA 循环搬运 256 点波表
 * 2025-12-03  完整版
 */
#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "I2C_GPIO_25a.h"
#include "OLED13.h"

/* ---------- 用户参数 ---------- */
#define WAVE_POINTS 256U                /* 必须是 2 的幂 */
#define SYSTEM_CLOCK_FREQ 8000000UL  // 8MHz系统时钟

static uint16_t gTriWave[WAVE_POINTS];  /* 波表 */
static uint16_t gAmp      = 4095;       /* 0-4095 → 0-3 V */
static uint16_t gUpStep   = 30;         /* 上升步长（1-100） */
static uint16_t gDnStep   = 30;         /* 下降步长（1-100） */

/* ---------- EC11 变量 ---------- */
volatile int32_t Ex_cnt   = 0;
volatile uint8_t keyRising= 0;

/* ---------- 菜单状态 ---------- */
typedef enum { SET_FREQ, SET_AMP, SET_UP_DUTY, SET_DN_DUTY, SET_EXIT } SetMode_t;
static SetMode_t gMode    = SET_FREQ;
static uint8_t   inSetMode= 0;
/* 限幅宏 */
#define CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))

/* 刷新第 y 页（0-7）整行，不整屏死等 */
static void OLED_FlushLine(uint8_t y)
{
    OLED_WR_Byte(0xB0 | y, OLED_CMD);
    OLED_WR_Byte(0x02, OLED_CMD);   // 列低 0
    OLED_WR_Byte(0x10, OLED_CMD);   // 列高 0
    for (uint8_t x = 0; x < 128; x++)
        OLED_WR_Byte(OLED_GRAM[y][x], OLED_DATA);
}
/* 百分数 → DAC 码 */
static uint16_t percent_to_dac(uint8_t pct)
{
    return (uint32_t)pct * 4095 / 100;
}

/* DAC 码 → 百分数 */
static uint8_t dac_to_percent(uint16_t dac)
{
    return (uint32_t)dac * 100 / 4095;
}
/* ---------- 4 MHz 定时器频率计算 ---------- */
static void set_tri_wave_freq(uint32_t freq_hz)
{
//    if (freq_hz < 20)  freq_hz = 20;
//    if (freq_hz > 40000) freq_hz = 400;
    uint32_t period = SYSTEM_CLOCK_FREQ/ (freq_hz * WAVE_POINTS) - 1U;
    DL_TimerA_stopCounter(TIMER_0_INST);
    DL_TimerA_setLoadValue(TIMER_0_INST, period);
    DL_TimerA_startCounter(TIMER_0_INST);
}

/* ---------- 生成任意占空比三角波表 ---------- */
static void rebuild_tri_wave(void)
{
    uint16_t i;
    uint16_t riseEnd = (WAVE_POINTS * gUpStep) / (gUpStep + gDnStep);
    if (riseEnd == 0) riseEnd = 1;
    if (riseEnd >= WAVE_POINTS) riseEnd = WAVE_POINTS - 1;

    for (i = 0; i < riseEnd; i++)
        gTriWave[i] = (uint32_t)gAmp * i / riseEnd;
    for (; i < WAVE_POINTS; i++)
        gTriWave[i] = gAmp - (uint32_t)gAmp * (i - riseEnd) / (WAVE_POINTS - riseEnd);
}

/* ---------- DMA 循环初始化 ---------- */
static void tri_wave_dma_init(void)
{
    DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
    DL_DMA_setSrcAddr     (DMA, DMA_CH0_CHAN_ID, (uint32_t)gTriWave);
    DL_DMA_setDestAddr    (DMA, DMA_CH0_CHAN_ID, (uint32_t)&(DAC0->DATA0));
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, WAVE_POINTS);
    DL_DMA_enableChannel  (DMA, DMA_CH0_CHAN_ID);
}

/* ---------- QEI 仅计步 ---------- */
static uint32_t lastQEI = 0;
static void updateQEI(void)
{
    uint32_t q = DL_TimerG_getTimerCount(TIMER_QEI_INST);
    if (q != lastQEI) { int32_t d = (int32_t)(q - lastQEI); Ex_cnt += d; lastQEI = q; }
}

/* ---------- OLED 菜单显示 ---------- */
static void showSetMenu(void)
{
    OLED_Clear();                                // 清 GRAM
    char line[20];
    const char *name[] = {"Freq", "Amp", "Up%", "Dn%"};
    snprintf(line, sizeof(line), "Set %s", name[gMode]);
    OLED_ShowString(0, 0, line);                 // 第 0 页

    switch (gMode) {
    case SET_FREQ: {
        uint32_t hz = SYSTEM_CLOCK_FREQ / (DL_TimerA_getLoadValue(TIMER_0_INST)+1) / WAVE_POINTS;
        snprintf(line, sizeof(line), "%u Hz", hz);
        break;
    }
    case SET_AMP:
        snprintf(line, sizeof(line), "%u%%", dac_to_percent(gAmp));
        break;
    case SET_UP_DUTY:
        snprintf(line, sizeof(line), "Up:%u%%", gUpStep);
        break;
    case SET_DN_DUTY:
        snprintf(line, sizeof(line), "Dn:%u%%", gDnStep);
        break;
    default: break;
    }
    OLED_ShowString(0, 2, line);                 // 第 2 页

    /* 只刷 0 和 2 页 → 256 byte，不死等 */
    OLED_FlushLine(0);
    OLED_FlushLine(1);
     OLED_FlushLine(2);
     OLED_FlushLine(3);

}
void TIMER_QEI_INST_IRQHandler(void)
{
    // 清除所有可能的中断标志
    uint32_t int_status =  DL_TimerG_getPendingInterrupt(TIMER_QEI_INST);
    DL_TimerG_clearInterruptStatus(TIMER_QEI_INST, int_status);

    // 安全地更新QEI
    updateQEI();
}
/* ---------- 应用 EC11 修改 ---------- */
static void applyEC11(void)
{
    int32_t step = Ex_cnt; Ex_cnt = 0;
    if (step == 0) return;
    int8_t dir  = step > 0 ? 1 : -1;
    int32_t abs = step > 0 ? step : -step;

    switch (gMode) {
    case SET_FREQ: {
        static uint16_t hz = 100;
        hz += dir * abs * 5;
        hz = CLAMP(hz, 20, 400);
        set_tri_wave_freq(hz);
        break;
    }
    case SET_AMP: {
        uint8_t pct = dac_to_percent(gAmp);
        pct += dir * abs * 5;          // 5% 步进
        pct = CLAMP(pct, 0, 100);
        gAmp = percent_to_dac(pct);
        rebuild_tri_wave();
        break;
    }
    case SET_UP_DUTY: {
        gUpStep += dir * abs;          // 1% 步进
        gUpStep = CLAMP(gUpStep, 1, 99);
        rebuild_tri_wave();
        break;
    }
    case SET_DN_DUTY: {
        gDnStep += dir * abs;
        gDnStep = CLAMP(gDnStep, 1, 99);
        rebuild_tri_wave();
        break;
    }
    default: break;
    }
    showSetMenu();        // 实时刷新
}

/* ---------- EC11 按键中断 ---------- */
void GROUP1_IRQHandler(void)
{
    if (DL_GPIO_getPendingInterrupt(GPIOA) & GPIO_EC11_SW_IIDX)
    {
        DL_GPIO_clearInterruptStatus(GPIOA, GPIO_EC11_SW_IIDX);
        keyRising = 1;
        //DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_PIN_0_PIN);
        DL_GPIO_togglePins(GPIO_SET_PORT, GPIO_SET_PIN_1_PIN); // ← 怀疑的
    }
}

/* ---------- 主界面刷新 ---------- */
static uint8_t mainUIFresh = 1;
static void showMainUI(void)
{

    if (!mainUIFresh) return;
    mainUIFresh = 0;
    OLED_Clear();
    OLED_Refresh();
    OLED_ShowString(0, 0, "EC11 TriWave");
    char line[20];
    uint16_t load = DL_TimerA_getLoadValue(TIMER_0_INST) + 1;
    uint16_t hz   = SYSTEM_CLOCK_FREQ / load / WAVE_POINTS;
    snprintf(line, sizeof(line), "F:%u Hz", hz);
    OLED_ShowString(0, 2, line);
    snprintf(line, sizeof(line), "A:%u%%", (uint32_t)gAmp * 100 / 4095);
    OLED_ShowString(0, 4, line);
    snprintf(line, sizeof(line), "U:D=%u:%u", gUpStep, gDnStep);
    OLED_ShowString(0, 6, line);

    OLED_Refresh();


}

/* ---------- 主函数 ---------- */
int main(void)
{
    SYSCFG_DL_init();
    DL_GPIO_disableInterrupt(GPIOA, GPIO_OLED_13_GPIO_SCK_PIN |
                                       GPIO_OLED_13_GPIO_SDA_PIN);
    I2CB1_Init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"EC11 TriWave");
    //OLED_Refresh();
    NVIC_SetPriority(GPIO_EC11_INT_IRQN, 7);   // 0 最高，7 最低
    rebuild_tri_wave();
    tri_wave_dma_init();
    set_tri_wave_freq(86);

    NVIC_EnableIRQ(TIMER_QEI_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_EC11_INT_IRQN);
    DL_TimerG_startCounter(TIMER_QEI_INST);
    lastQEI = DL_TimerG_getTimerCount(TIMER_QEI_INST);

    while (1)
    {
        updateQEI();

        /* 短按处理 */
        if (keyRising) {
            keyRising = 0;
            if (!inSetMode) {                // 第一次进入
                inSetMode = 1;
                gMode     = SET_FREQ;
                showSetMenu();
            } else {                         // 已在菜单
                gMode = (gMode + 1) % (SET_EXIT + 1); // 循环切项
                if (gMode == SET_EXIT) {     // 最后一项 → 保存并退出
                    inSetMode = 0;
                    mainUIFresh = 1;
                    OLED_Clear();
                    OLED_ShowString(0, 0, "Saved!");
                    OLED_Refresh();
                    //delay_cycles(500000);
                } else {
                    showSetMenu();
                }
            }
        }

        /* 旋转处理 */
        if (inSetMode && Ex_cnt != 0) applyEC11();
        if (!inSetMode) showMainUI();

        __WFI();
    }
}
