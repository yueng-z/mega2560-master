#include "dino_Flag.h"
#include "oled.h"
#include "dino_ShareData.h"
#include "dino_level.h"

unsigned char		flag_y = 43;

//旗杆
const unsigned char  flagpole[7][4] ={
	0x06,0xFF,0xFF,0x06,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x00,
};
//旗帜
const unsigned char  flag[8][18] ={
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x01,0x02,0x04,0x08,0x18,0x2E,0x5B,0xBF,
	0x40,0xC0,0x40,0x40,0x40,0x40,0x40,0xC0,0xC0,0x00,0x00,0x01,0x02,0x04,0x0C,0x17,0x2D,0x5F,
	0x20,0x60,0xA0,0x20,0x20,0x20,0xA0,0xE0,0xE0,0x00,0x00,0x00,0x01,0x02,0x06,0x0B,0x16,0x2F,
	0x10,0x30,0x50,0x90,0x10,0x10,0xD0,0x70,0xF0,0x00,0x00,0x00,0x00,0x01,0x03,0x05,0x0B,0x17,
	0x08,0x18,0x28,0x48,0x88,0x88,0xE8,0xB8,0xF8,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x05,0x0B,
	0x04,0x0C,0x14,0x24,0x44,0xC4,0x74,0xDC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x05,
	0x02,0x06,0x0A,0x12,0x22,0x62,0xBA,0x6E,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,
	0x01,0x03,0x05,0x09,0x11,0x31,0x5D,0xB7,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
};

//加载旗帜
char load_flag()
{
	if(flag_pos[0]- 2 >= screen_left - 18 && flag_pos[0] + 15 <= screen_right + 18) return 1;
	else if(flag_pos[0]- 2 >= screen_left - 28 && flag_pos[0] + 15 <= screen_right + 28) return 2;
	else return 0;
}

//绘制旗帜
void OLED_DrawFlag() 
{
	char flag_sign;
	char x,y,i;
	char setx;
	char draw_start_x;
	char Pull = 0;
	unsigned char flag_deep = 0;
	static char bounce = 0;

	static int last_screen_x;
	static int last_screen_y;
	static signed char last_deep;
	int* p1 = &last_screen_x;
	int* p2 = &last_screen_y;
	signed char* p3 = &last_deep;

	static int last_screen_x1;
	static int last_screen_y1;
	static signed char last_deep1;
	int* p11 = &last_screen_x1;
	int* p21 = &last_screen_y1;
	signed char* p31 = &last_deep1;

	flag_sign = load_flag();

	if (flag_sign == 1)
	{
		if (Rise == 1)
		{
			if (dino_x + 15 + screen_left >= flag_pos[0])
			{
				dino_x = flag_pos[0] - 16 -screen_left;
				Pull = 1;
			}

			if (Pull == 1)
			{
				flag_y -= 5;
			}
			else if(flag_y < 43)
			{
				flag_y += 3;
			}
			else if (flag_y > 43)
			{
				flag_y = 43;
			}
			
			if (flag_y <= 8 )
			{
				flag_y = 8;
				Rise = 0;
				x_sta = -5;
				y_sta = 4;
				foothold = 0;
				jumping = 1;
				control = 1;
				bounce = y_sta + 1;
				score+=1000;
                life++;
				UART1_send(mla_1up,6);
			}

		}
		flag_deep = (63 - (flag_y + 15))/8;

		if (flag_pos[0] - 2 - screen_left <= 0)
		{
			setx = 0;
			draw_start_x = -(flag_pos[0] - 2 - screen_left);
		}
		else
		{
			setx = flag_pos[0] - 2 - screen_left;
			draw_start_x = 0;
		}
		for (y = 0; y < 2; y++)
		{
			oledSetPos(setx, flag_deep + y);
			for (x = draw_start_x; x < 9; x++)
			{
				if (flag_pos[0] - 2 - screen_left + x > 127)
				{
					break;
				}
				i = 9*y + x;
				oledWriteByte(flag[(flag_y)%8][i],OLED_DATA);
			}
		}
		OLED_shadowclear(flag_pos[0] - 2 - screen_left, flag_y, flag_deep, 9, 16, p1, p2, p3, normalcy);
		

		if (flag_pos[0] + 7 - screen_left <= 0)
		{
			setx = 0;
			draw_start_x = -(flag_pos[0] + 7 - screen_left);
		}
		else
		{
			setx = flag_pos[0] + 7 - screen_left;
			draw_start_x = 0;
		}
		for (y = 0; y < 7; y++)
		{

			oledSetPos(setx, y);
			for (x = draw_start_x; x < 4; x++)
			{
				if (flag_pos[0] + 7 - screen_left + x > 127)
				{
					break;
				}
				oledWriteByte(flagpole[y][x],OLED_DATA);
			}
		}
		

		oledSetPos(flag_pos[0]- screen_left, 7);
		// for (x = 0; x < 16; x++)
		// {
			
		// 	oledWriteByte(flagpole[i],OLED_DATA);
		// }
		if (bounce > 0)
		{
			//printf("dino_yspeed = %d",dino_yspeed);
			bounce--;
			if (bounce == 0)
			{
				y_sta = 0;
				jumping = 0;
				control = 0;
			}
		}
		
		OLED_shadowclear(flag_pos[0] + 7 - screen_left, 8, 0, 4, 56, p11, p21, p31, normalcy);
		
	}
	else if (flag_sign == 2)
	{
		OLED_shadowclear(flag_pos[0] - 2 - screen_left, flag_y, flag_deep, 9, 16, p1, p2, p3, normalcy);
		OLED_shadowclear(flag_pos[0] + 7 - screen_left, 8, 0, 4, 56, p11, p21, p31, normalcy);
	}

	
}