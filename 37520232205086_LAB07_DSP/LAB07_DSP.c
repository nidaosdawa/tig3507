#include "arm_const_structs.h"   // CMSIS-DSP 预定义常数表（如 arm_cfft_sR_q15_len256）
#include "arm_math.h"
#include "ti_msp_dl_config.h"
#include "OLED13.h"
#include "I2C_GPIO_25a.h"
#include "stdio.h"

#define ADC_POINT   1024
#define FS          20000UL
#define VREF        3.3f

static int16_t  adcBuf[ADC_POINT];              // 实数采样
static float32_t  fftIn_f32[ADC_POINT * 2];     // 复数输入（实部/虚部）
static float32_t  fftMag_f32[ADC_POINT*2];        // 幅值谱
/* Maximum amplitude of FFT */
volatile float32_t gFFTmaxValue;
/* Index of maximum frequency */
volatile uint32_t gFFTmaxFreqIndex;
static float32_t hanningWindow[ADC_POINT];

/* ---------- 标志位 ---------- */
volatile bool  sampleDone = false;

static uint32_t gTicks = 0;          // 1 ms 计数器
static bool     gRefresh = false;    // 1 秒到点刷新标志

/* ---------- 函数原型 ---------- */
//static void buildHanning_q15(void);
arm_cfft_radix4_instance_f32 scfft;
static void signal_process_real_float(void);
static void OLED_display(float32_t freq, float32_t amp);
/* ---------- 信号处理 ---------- */
uint32_t tmpIdx;
int16_t  tmpVal;
int16_t dc;

static void signal_process_real_float(void)
{
    /* 1. 中点归零 → 伏特（无窗）*/
    float dc = 2048.0f;
    for (int i = 0; i < ADC_POINT; i++) {
        float v = ((float)adcBuf[i] - dc) * (VREF / 4096.0f);
        fftIn_f32[i*2]   = v;
        fftIn_f32[i*2+1] = 0.0f;
    }

    /* 2. radix-4 FFT */
    arm_cfft_radix4_init_f32(&scfft, 1024, 0, 1);
    arm_cfft_radix4_f32(&scfft, fftIn_f32);

    /* 3. 幅值 + 归一化 + 单边谱 */
    arm_cmplx_mag_f32(fftIn_f32, fftMag_f32, ADC_POINT);
    for (int i = 0; i < ADC_POINT; i++) fftMag_f32[i] /= ADC_POINT;
    for (int i = 1; i < ADC_POINT/2; i++) fftMag_f32[i] *= 2.0f;

    /* 4. 最大峰（跳过 DC）*/
    float maxVal;
    uint32_t maxIdx;
    arm_max_f32(&fftMag_f32[1], ADC_POINT/2 - 1, &maxVal, &maxIdx);
    gFFTmaxValue   = maxVal;        // 直接伏特
    gFFTmaxFreqIndex = maxIdx + 1;

}

int main(void)
{
    /* 初始化 */
    SYSCFG_DL_init();
    OLED_Init();
    //DMA设置
       DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID,(uint32_t) 0x40556280);
       DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID,(uint32_t)adcBuf);
       DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, ADC_POINT);

       DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

     //中断
     NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    // buildHanning_q15();
    /* 启动 ADC + DMA + Timer */
    DL_ADC12_enableConversions(ADC12_0_INST);

    DL_TimerG_startCounter(TIMER_0_INST);
 //   DL_TimerA_startCounter(TIMER_1_INST);


//    /* 1. 把 PA的复用功能切到 ADC1_CH0 */
    DL_GPIO_initPeripheralInputFunction(
            GPIO_ADC12_0_IOMUX_C3,          // IOMUX_PINCM37
        GPIO_ADC12_0_IOMUX_C3_FUNC       // 复用功能编号

    );
    OLED_ShowString(0, 0, "System Initialized");
       OLED_Refresh();

    while (1)
    {
       /* 等待 DMA 完成一组 1024点 */
        while (!sampleDone) { __WFE(); }
        sampleDone = false;
        /* FFT + 计算主频/幅度 */
//        OLED_ShowString(0, 2, "DMA Done");
//        OLED_Refresh();

        signal_process_real_float();
//        if (gRefresh){
        /* OLED 显示 */
        float32_t freq = gFFTmaxFreqIndex * FS / ADC_POINT;
        float32_t amp  = gFFTmaxValue;   // 已经是伏特！

        printf("idx=%u  raw=%.3f V\n",
               gFFTmaxFreqIndex,
               gFFTmaxValue);
        OLED_display(freq, amp);
        //delay_cycles(4000000);

//    }
        DL_TimerG_startCounter(TIMER_0_INST);
        DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
    }
}


/* ---------- DMA 完成中断 ---------- */
//void ADC12_0_INST_IRQHandler(void)
//{
//    if (DL_ADC12_getPendingInterrupt(ADC12_0_INST) == DL_ADC12_IIDX_DMA_DONE) {
//        sampleDone = true;
//    }
//}


void ADC12_0_INST_IRQHandler(void)
{
    if (DL_ADC12_getPendingInterrupt(ADC12_0_INST) == DL_ADC12_IIDX_DMA_DONE) {
        /* 注入 4 kHz 方波测试信号 */

        sampleDone = true;
    }
}

///* ---------- 生成 Hanning 窗（Q15） ---------- */
//static void buildHanning_q15(void)
//{
//    for (int i = 0; i < ADC_POINT; i++) {
//        float32_t h = 0.5f - 0.5f * arm_cos_f32(2.0f * PI * i / (ADC_POINT - 1));
//        hanningWindow[i] = (int16_t)(h * 32768.0f);
//    }
//}


/* ---------- OLED 显示 ---------- */
static void OLED_display(float32_t freq, float32_t amp)
{
    char buf[32];
    OLED_Clear();
    OLED_ShowString(0, 0, "Freq:");
    snprintf(buf, sizeof(buf), "%5.2f Hz", freq);
    OLED_ShowString(0, 2, buf);
    OLED_ShowString(0, 4, "Amp:");
    snprintf(buf, sizeof(buf), "%5.2f V", amp);
    OLED_ShowString(0, 6, buf);

    //OLED_Refresh();
}


