#include "oled.h"
#include "oledfont.h"
#include "board.h"
#include "Delay.h"

#if OLED_MODE == 1

void oledWriteByte(u8 dat, u8 cmd)
{
	DATAOUT(dat);
	if (cmd)
		OLED_DC_Set();
	else
		OLED_DC_Clr();
	OLED_CS_Clr();
	OLED_WR_Clr();
	OLED_WR_Set();
	OLED_CS_Set();
	OLED_DC_Set();
}
#else

void oledWriteByte(u8 dat, u8 cmd)
{
	u8 i;
    CLI();
	if (cmd)
	{
		OLED_DC_Set();
	}
	else if (cmd != 1)
	{
		OLED_DC_Clr();
	}
	OLED_CS_Clr();
	spitransferByte(dat);
	OLED_CS_Set();
	//OLED_DC_Set();
    OLED_DC_Clr();
    SEI();
}
#endif
void oledSetPos(unsigned char x, unsigned char y)
{
	oledWriteByte(0xb0 + y, OLED_CMD);
	oledWriteByte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	//oledWriteByte((x & 0x0f) | 0x01, OLED_CMD);
    oledWriteByte((x & 0x0f), OLED_CMD);
}

void oledDisplayOn(void)
{
	oledWriteByte(0X8D, OLED_CMD); // SET DCDC
	oledWriteByte(0X14, OLED_CMD); // DCDC ON
	oledWriteByte(0XAF, OLED_CMD); // DISPLAY ON
}

void oledDisplayOff(void)
{
	oledWriteByte(0X8D, OLED_CMD); // SET DCDC
	oledWriteByte(0X10, OLED_CMD); // DCDC OFF
	oledWriteByte(0XAE, OLED_CMD); // DISPLAY OFF
}
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	
void oledClear(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		oledWriteByte(0xb0 + i, OLED_CMD); //����ҳ��ַ��0~7��
		oledWriteByte(0x00, OLED_CMD);	  //������ʾλ�á��е͵�ַ
		oledWriteByte(0x10, OLED_CMD);	  //������ʾλ�á��иߵ�ַ   
		for (n = 0; n < 128; n++)
			oledWriteByte(0, OLED_DATA);
	}//������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~8
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/8

void oledShowChar(u8 x, u8 y, u8 chr)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; //�õ�ƫ�ƺ��ֵ	
	if (x > Max_Column - 1)
	{
		x = 0;
		y = y + 2;
	}
	if (SIZE == 16)
	{
		oledSetPos(x, y);
		for (i = 0; i < 8; i++)
			oledWriteByte(F8X16[c * 16 + i], OLED_DATA);
		oledSetPos(x, y + 1);
		for (i = 0; i < 8; i++)
			oledWriteByte(F8X16[c * 16 + i + 8], OLED_DATA);
	}
	else
	{
		oledSetPos(x, y + 1);
		for (i = 0; i < 6; i++){
		   // oledSetPos(x+i, y + 1);
			oledWriteByte(F6x8[c][i], OLED_DATA);
		}
	}

}
//m^n����
u32 oled_pow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	

void oledShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
{
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				oledShowChar(x + (size2 / 2) * t, y, ' ');
				continue;
			}
			else
				enshow = 1;
		}
		oledShowChar(x + (size2 / 2) * t, y, temp + '0');
	}
}

void oledShowString(u8 x, u8 y, u8 *chr)//(0,0."hello world")
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{
		oledShowChar(x, y, chr[j]);
		x += 8;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		j++;
	}
}


void oledShowCHinese(u8 x, u8 y, u8 no)
{
	u8 t, adder = 0;
	oledSetPos(x, y);
	for (t = 0; t < 16; t++)
	{
		oledWriteByte(Hzk[2 * no][t], OLED_DATA);
		adder += 1;
	}
	oledSetPos(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		oledWriteByte(Hzk[2 * no + 1][t], OLED_DATA);
		adder += 1;
	}
}

//oledShowCHStr(0,0,"����Ƕ��ʽ");
void oledShowCHStr(unsigned char X , unsigned char Y, unsigned char *pstr)
{
	unsigned char i;
	unsigned char Addr = 0;
	unsigned char count = 0;

	while(*pstr)
	{
		for(i=0; i<chinese_12_len; i++)    //���ֿ��и��ݺ���GBK����Ѱ�Һ��ֵ���ģ
		{
			if((*pstr == chinese_12[i].value[0])&&(*(pstr+1) == chinese_12[i].value[1]))
			{
				Addr = i;                      //Addr �������ֿ��е��±�
				break;
			}				
		}		
		oledSetPos(X+count*12,Y);                             //���ó�ʼλ��
		for (i=0; i<12; i++)              //д�ϰ벿��
		{ 	
			oledWriteByte(chinese_12[Addr].data[i],1);
		}
		oledSetPos(X+count*12,Y+1);    
		for (i=0; i<12; i++)              //дһ������
		{ 	
			oledWriteByte(chinese_12[Addr].data[12+i],1);
		}
		count++;                          //����ʾ������	
		pstr++;
		pstr++;  	
	}	  
}
/***********功能杝述：显示显示BMP图片128×64起始点坝标(x,y),x的范围0～127，y为页的范围0～7*****************/
void oledDrawBMP(unsigned char xs, unsigned char ys, unsigned char xw, unsigned char yh, const unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y, yend;

	if (yh % 8 == 0)
		yend = ys + yh / 8;
	else
		yend = ys + yh / 8 + 1;
	for (y = ys; y < yend; y++)
	{
		oledSetPos(xs, y);
		for (x = xs; x < xs + xw; x++)
		{
			oledWriteByte(BMP[j++], OLED_DATA);
		}
	}
}


void Port_init(void)
{

 PORTB |= 0x06;
 DDRB  |= 0x06;
 PORTG |= 0x04;
 DDRG  |= 0x04;
 PORTL |= 0x48;
 DDRL  |= 0x48;
}
// 3?��??��SSD1306
void OLED_Init(void)
{
	Port_init();
	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set();

	oledWriteByte(0xAE, OLED_CMD); //--turn off oled panel
	oledWriteByte(0x00, OLED_CMD); //---set low column address
	oledWriteByte(0x10, OLED_CMD); //---set high column address
	oledWriteByte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oledWriteByte(0x81, OLED_CMD); //--set contrast control register
	oledWriteByte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
	oledWriteByte(0xA1, OLED_CMD); //--Set SEG/Column Mapping      0xa0���ҷ��� 0xa1����
	oledWriteByte(0xC8, OLED_CMD); // Set COM/Row Scan Direction    0xc0���·��� 0xc8����
	oledWriteByte(0xA6, OLED_CMD); //--set normal display
	oledWriteByte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	oledWriteByte(0x3f, OLED_CMD); //--1/64 duty
	oledWriteByte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	oledWriteByte(0x00, OLED_CMD); //-not offset
	oledWriteByte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
	oledWriteByte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
	oledWriteByte(0xD9, OLED_CMD); //--set pre-charge period
	oledWriteByte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oledWriteByte(0xDA, OLED_CMD); //--set com pins hardware configuration
	oledWriteByte(0x12, OLED_CMD);
	oledWriteByte(0xDB, OLED_CMD); //--set vcomh
	oledWriteByte(0x40, OLED_CMD); // Set VCOM Deselect Level
	oledWriteByte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
	oledWriteByte(0x02, OLED_CMD); //
	oledWriteByte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
	oledWriteByte(0x14, OLED_CMD); //--set(0x10) disable
	oledWriteByte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
	oledWriteByte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
	oledWriteByte(0xAF, OLED_CMD); //--turn on oled panel

	oledWriteByte(0xAF, OLED_CMD); /*display ON*/
	oledClear();
	oledSetPos(0, 0);
}

u8 OLED_GRAM[OLED_W][GRAM_H];
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}
void OLED_Refresh(void){
	u8 i, n;
	for (i = 0; i < OLED_H/8; i++)
	{
		oledWriteByte(0xb0 + i, OLED_CMD); //设置页地址�?0~7�?
		oledWriteByte(0x00, OLED_CMD);	  //设置显示佝置—列低地址
		oledWriteByte(0x10, OLED_CMD);	  //设置显示佝置—列高地址   
		for (n = 0; n < OLED_W; n++)
			oledWriteByte(OLED_GRAM[n][i], OLED_DATA);
	}//更新显示

}