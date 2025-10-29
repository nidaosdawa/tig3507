#ifndef __OLED13_H
#define __OLED13_H

#include "ti_msp_dl_config.h"
#include <stdint.h>

// ==========================================================================
// OLED 参数定义
#define  OLED_CMD  0    // 写命令
#define  OLED_DATA 1    // 写数据
#define  SIZE 16
#define  XLevelL        0x02
#define  XLevelH        0x10
#define  Max_Column     128 // 液晶屏“列”数量
#define  Max_Row        64  // 液晶屏“行”数量
#define  Brightness     0xFF
//#define  X_WIDTH        128 // 液晶屏“像素数”：宽度
//#define  Y_WIDTH        64  // 液晶屏“像素数”：高度

// ==========================================================================
// OLED 控制函数
extern void OLED_Display_On(void);
extern void OLED_Display_Off(void);
extern void OLED_Init(void);
extern void OLED_Clear(void);
extern void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
extern void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
extern void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
extern void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2);
extern void OLED_ShowString(uint8_t x,uint8_t y, char *p);
extern void OLED_Set_Pos(uint8_t x, uint8_t y);
extern void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
extern void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[]);
extern void OLED_Refresh(void);

#endif

// ==========================================================================
// End of file.
