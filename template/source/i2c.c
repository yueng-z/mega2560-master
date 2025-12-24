// ***************     I2C driver V 1.0    ***************
// ***           Written By P. Fletcher-Jones          ***
// ***                Written on 16/11/01			   ***
// ***                 Last MOD 21/11/01			   ***
// ***       Compiled using ImageCraft C Comiler       ***
// *******************************************************

#include "board.h"
#include "I2C.h"
#include "timer.h"

void I2C_init(void)
{
  SCL_OutputLow;
  SDA_OutputLow;
  SCL_Low;
  SDA_Low;
}

//产生启动信号
unsigned char I2C_start(void)
{
  SDA_Hight;
  I2C_Delay;
  SCL_Hight;
  I2C_Delay;
  SDA_Low;
  I2C_Delay;
  SCL_Low;
  I2C_Delay;
  return 1;
}

//产生停止信号
void I2C_stop(void)
{
  SDA_Low;
  I2C_Delay;
  SCL_Hight;
  I2C_Delay;
  SDA_Hight;
  I2C_Delay;
}

//向总线写一字节,并返回有无应答
unsigned char I2C_write(unsigned char c)
{
  unsigned char i, ack = 1;
  for (i = 0; i < 8; i++)
  {
    if (c & 0x80)
      SDA_Hight;
    else
      SDA_Low;
    SCL_Hight;
    I2C_Delay;
    SCL_Low;
    c <<= 1;
    I2C_Delay;
  }
  SDA_Hight;
  I2C_Delay;
  SCL_Hight;
  I2C_Delay;
  if (SDA_Input)
    ack = 0; //失败
  SCL_Low;
  I2C_Delay;
  return ack;
}

//读一字节 ack: 1时应答，0时不应答
unsigned char I2C_read(unsigned char ack)
{
  unsigned char i, ret;
  SDA_Hight;
  for (i = 0; i < 8; i++)
  {
    I2C_Delay;
    SCL_Low;
    I2C_Delay;
    SCL_Hight;
    I2C_Delay;
    ret <<= 1;
    if (SDA_Input)
      ret++;
  }
  SCL_Low;
  I2C_Delay;
  if (ack) //非应答
    SDA_Low;
  else //应答
    SDA_Hight;
  I2C_Delay;
  SCL_Hight;
  I2C_Delay;
  SCL_Low;
  I2C_Delay;
  return (ret);
}

// read a byte from the 24C256
unsigned char eeprom_read(unsigned int address)
{
  unsigned char data;
  I2C_start();                       // 发起始信号
  I2C_write(EEPROM_BUS_ADDRESS);     // 发写从机写寻址字节
  I2C_write(address >> 8);           // 发存储单元地址高字节
  I2C_write(address);                // 发存储单元地址低字节
  I2C_start();                       // 发起始信号
  I2C_write(EEPROM_BUS_ADDRESS | 1); // 发从机读寻址字节
  data = I2C_read(0);                // 读一个字节数据，返回NO ACK
  I2C_stop();                        // 发停止信号
  return data;
}

// write a byte to the 24C256
void eeprom_write(unsigned int address, unsigned char data)
{
  int eeprom_timer;
  I2C_start();                   // 发起始信号
  I2C_write(EEPROM_BUS_ADDRESS); // 发写从机写寻址字节
  I2C_write(address >> 8);       // 发存储单元地址高字节
  I2C_write(address);            // 发存储单元地址低字节
  I2C_write(data);               // 写一个字节数据到24C256
  I2C_stop();                    // 发停止信号
  eeprom_timer = Timer_GetTickCount();
  while (!Timer_PassedDelay(eeprom_timer, 5))
    ; // 等待5ms，保证EEPROM内部写操作完成再进行新操作
}