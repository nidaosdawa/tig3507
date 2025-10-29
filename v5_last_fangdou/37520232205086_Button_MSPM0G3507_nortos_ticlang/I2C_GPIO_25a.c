#include "I2C_GPIO_25a.h"
#include "ti_msp_dl_config.h"

// ==========================================================================
// I2C �ӿ� OLED Һ��ģ���뵥Ƭ�����ߣ����ţ�
// ��Ϊ��������ʱ�����޸����¼�������:
// OLED_I2C_SCL_H(); OLED_I2C_SCL_L(); OLED_I2C_SDA_H(); OLED_I2C_SDA_L()
//  GND     ��Դ��
//  VCC     �� 3.3v ��Դ
//  SCK     PA31 ��ʱ�ӣ�
//  SDA     PA28 �����ݣ�

/* The delay assuming 32MHz CPU_CLK */
#define DELAY2 (160) // delay 160 Cycles

#define POWER_STARTUP_DELAY                                                (16)

// ==========================================================================
// I2C_SCL �����ͷţ��ⲿ��������Ϊ�ߵ�ƽ��
void OLED_I2C_SCL_H(void)
{
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SCK_IOMUX);
    DL_GPIO_setPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SCK_PIN);
    delay_cycles(DELAY2);
}
// I2C_SCL ��������͵�ƽ
void OLED_I2C_SCL_L(void)
{
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SCK_IOMUX);
    DL_GPIO_clearPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SCK_PIN);
    delay_cycles(DELAY2);
}
// I2C_SDA �����ͷţ��ⲿ��������Ϊ�ߵ�ƽ��
void OLED_I2C_SDA_H(void)
{
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SDA_IOMUX);
    DL_GPIO_setPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SDA_PIN);
    delay_cycles(DELAY2);
}
// I2C_SDA ��������͵�ƽ
void OLED_I2C_SDA_L(void)
{
    DL_GPIO_initDigitalOutput(GPIO_OLED13_GPIO_SDA_IOMUX);
    DL_GPIO_clearPins(GPIO_OLED13_PORT, GPIO_OLED13_GPIO_SDA_PIN);
    delay_cycles(DELAY2);
}
// ==========================================================================
// ��ʼ�� I2C ģ��
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

    OLED_I2C_SCL_L();    // I2C_SCL �͵�ƽ
    OLED_I2C_SDA_H();    // I2C_SDA �ߵ�ƽ
    OLED_I2C_SCL_H();    // I2C_SCL �ߵ�ƽ
}
// ==========================================================================
void IIC_Start()
{
    OLED_I2C_SCL_H();
    OLED_I2C_SDA_H();
    OLED_I2C_SDA_L();
    OLED_I2C_SCL_L();
}

// ==========================================================================
// IIC Stop
void IIC_Stop()
{
    OLED_I2C_SCL_H();
    OLED_I2C_SDA_L();
    OLED_I2C_SDA_H();
}
// ==========================================================================
void IIC_Wait_Ack()
{
    OLED_I2C_SCL_H();
    OLED_I2C_SCL_L();
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

