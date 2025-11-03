#include "ti_msp_dl_config.h"
#include "stdio.h"
#include <string.h>
#include "I2C_GPIO_25a.h"
#include "OLED13.h"
#include "OLED_BMP.h"       // 图片表，不显示图片时，可不包含本文件

/* 全局帧缓冲 */
#define RX_BUF_LEN  64
volatile uint8_t rxBuf[RX_BUF_LEN];
volatile uint8_t rxCnt = 0;
volatile bool    rxDone = false;
//串口重定向：把“标准输出”桥接到了串口——每次 printf 打一个字符，它就打包成 1 字节数组，通过厂商驱动塞进 UART0 发走

int fputc(int ch, FILE* stream)
 {
 if (ch == '\n')
     DL_UART_Main_transmitDataBlocking(UART_0_INST, '\r');
     DL_UART_Main_transmitDataBlocking(UART_0_INST, ch);
     return ch;

 }
/* 字符串整块输出，效率更高 */
int fputs(const char* restrict s, FILE* restrict stream)
 {
 uint16_t i, len;
 len = strlen(s);
 for(i=0; i<len; i++)
 {
 while (DL_UART_isBusy(UART_0_INST));
 DL_UART_transmitData(UART_0_INST, s[i]);
 }
 return len;
 }
int main(void)
{
    SYSCFG_DL_init();
   NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
   NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
   OLED_Init();
   printf("%s\r\n","version_1");
   printf("%s\r\n","editor:dongyunting");

       char ask[]   = "What's your name?\r\n";
       char reply[] = "My name is G3507\r\n";
       /* 上电先发问题 */
       for (char *p = ask; *p; p++)
           DL_UART_Main_transmitDataBlocking(UART_0_INST, *p);

       while (1)
       {
           if (rxDone) {                      // 收到完整一行
               rxDone = false;

               /* 比较时去掉回车换行 */
               if (strcmp((char *)rxBuf, "My name is G3507") == 0) {
                   OLED_ShowString(0, 0, "Name=G3507.");
               }
               /* 如果对方又问回来，再发一次 */
               else if (strcmp((char *)rxBuf, "What's your name") == 0) {
                   for (char *p = ask; *p; p++)
                       DL_UART_Main_transmitDataBlocking(UART_0_INST, *p);
               }

               rxCnt = 0;                       // 准备下一帧
               memset((char *)rxBuf, 0, RX_BUF_LEN);   /* 可选，更干净 */
           }
       }
   }


void UART_0_INST_IRQHandler(void){
 if (DL_UART_Main_getPendingInterrupt(UART_0_INST) == DL_UART_MAIN_IIDX_RX)
                {
                    uint8_t ch = DL_UART_Main_receiveData(UART_0_INST);
                    if (ch == '\r')          /* 直接丢弃回车 */
                        return;
                    if (ch == '\n') {               // 行结束
                        rxBuf[rxCnt] = 0;          // 加字符串结束符
                        rxDone = true;             // 通知主循环
                    }
                    else if (rxCnt < RX_BUF_LEN - 1) {
                        rxBuf[rxCnt++] = ch;
                    }
                }

}
