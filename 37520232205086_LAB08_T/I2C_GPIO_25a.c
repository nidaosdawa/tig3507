#include "I2C_GPIO_25a.h"
#include "ti_msp_dl_config.h"

// ==========================================================================
// I2C 接口 OLED 液晶模块与单片机连线（引脚）
// 改为其他引脚时，需修改如下几个函数:
// OLED_I2C_SCL_H(); OLED_I2C_SCL_L(); OLED_I2C_SDA_H(); OLED_I2C_SDA_L()
//  GND     电源地
//  VCC     接 3.3v 电源
//  SCK     PA31 （时钟）
//  SDA     PA28 （数据）

/* The delay assuming 32MHz CPU_CLK */
#define DELAY2 (160) // delay 160 Cycles

#define POWER_STARTUP_DELAY                                                (16)
/* 新增：读 SCLSDA 电平 */
static uint8_t OLED_I2C_SDA_Read(void)
{
    DL_GPIO_initDigitalInput(GPIO_OLED13_GPIO_SDA_IOMUX);
    uint8_t v = DL_GPIO_readPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SDA_PIN) ? 1 : 0;
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SDA_IOMUX);
    return v;
}
static uint8_t OLED_I2C_SCL_Read(void)
{
    /* 先把引脚改成输入，读电平，再恢复输出（模拟开漏）*/
    DL_GPIO_initDigitalInput(GPIO_OLED13_GPIO_SCK_IOMUX);   // 输入
    uint8_t lvl = DL_GPIO_readPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SCK_PIN) ? 1 : 0;
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SCK_IOMUX);  // 恢复输出
    return lvl;
}

// ==========================================================================
// I2C_SCL 引脚释放（外部电阻上拉为高电平）
void OLED_I2C_SCL_H(void)
{
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SCK_IOMUX);
    DL_GPIO_setPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SCK_PIN);
    delay_cycles(DELAY2);
}
// I2C_SCL 引脚输出低电平
void OLED_I2C_SCL_L(void)
{
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SCK_IOMUX);
    DL_GPIO_clearPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SCK_PIN);
    delay_cycles(DELAY2);
}
// I2C_SDA 引脚释放（外部电阻上拉为高电平）
void OLED_I2C_SDA_H(void)
{
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SDA_IOMUX);
    DL_GPIO_setPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SDA_PIN);
    delay_cycles(DELAY2);
}
// I2C_SDA 引脚输出低电平
void OLED_I2C_SDA_L(void)
{
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SDA_IOMUX);
    DL_GPIO_clearPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SDA_PIN);
    delay_cycles(DELAY2);
}
// ==========================================================================
// 初始化 I2C 模块
void I2CB1_Init(void)
{
    //DL_GPIO_reset(GPIOA);

    DL_GPIO_enablePower(GPIOA);
    delay_cycles(POWER_STARTUP_DELAY);
    
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SCK_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SDA_IOMUX);

    DL_GPIO_setPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SCK_PIN |
		GPIO_OLED13_GPIO_SDA_PIN);
    DL_GPIO_enableOutput(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SCK_PIN |
		GPIO_OLED13_GPIO_SDA_PIN);

    OLED_I2C_SCL_L();    // I2C_SCL 低电平
    OLED_I2C_SDA_H();    // I2C_SDA 高电平
    OLED_I2C_SCL_H();    // I2C_SCL 高电平
}
// IIC Stop
void IIC_Stop()
{
    OLED_I2C_SCL_H();
    OLED_I2C_SDA_L();
    OLED_I2C_SDA_H();
}
void IIC_Start()
{
    OLED_I2C_SCL_H();
    OLED_I2C_SDA_H();
    OLED_I2C_SDA_L();
    OLED_I2C_SCL_L();
}


// ==========================================================================
//void IIC_Wait_Ack()
//{
//    OLED_I2C_SCL_H();
//    OLED_I2C_SCL_L();
//}
void IIC_Wait_Ack(void)
{
    uint32_t to = 2000;                 // 40 us
    OLED_I2C_SCL_H();
    while (OLED_I2C_SDA_Read() != 0 && --to); // 等 SDA==0
    OLED_I2C_SCL_L();
    if (to == 0) return;                // 放弃本次传输
}
// ==========================================================================
// IIC Write byte
void Write_IIC_Byte(uint8_t IIC_Byte)
{
    uint8_t i;
    uint8_t m,data;
    data = IIC_Byte;
    OLED_I2C_SCL_L();
    for(i = 0; i < 8; i++)
    {
        m = data;
        m = m & 0x80;
        if(m == 0x80)
        {
            OLED_I2C_SDA_H();
        }
        else 
            OLED_I2C_SDA_L();
        data = data << 1;
        OLED_I2C_SCL_H();
        OLED_I2C_SCL_L();
    }
}
// ==========================================================================
// IIC Write Command
void Write_IIC_Command(uint8_t IIC_Command)
{
    IIC_Start();
    Write_IIC_Byte(OLED_I2C_ADR);   // Slave address
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00);           // write command
    IIC_Wait_Ack();    
    Write_IIC_Byte(IIC_Command); 
    IIC_Wait_Ack();    
    IIC_Stop();
}
// ==========================================================================
// IIC Write Data
void Write_IIC_Data(uint8_t IIC_Data)
{
    IIC_Start();
    Write_IIC_Byte(OLED_I2C_ADR);   // D/C#=0; R/W#=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40);           // write data
    IIC_Wait_Ack();    
    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();    
    IIC_Stop();
}

// ==========================================================================
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{    
    if(cmd)
    {
        Write_IIC_Data(dat);
    }
    else
    {
        Write_IIC_Command(dat);
    }
}

// ==========================================================================
// End of file.

