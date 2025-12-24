#include "dino_Sharemethod.h"
#include "dino_ShareData.h" 
#include "oled.h"

//坐标设置
void OLED_SetPos(unsigned char x, unsigned char y)
{
  oledSetPos(x,y);
}

// 快速绘制图像
void OLED_DrawBMPFast(const unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	for (y = 0; y < 8; y++)
	{
		OLED_SetPos(0, y);
		for (x = 0; x < 128; x++)
		{
			oledWriteByte(BMP[j++],OLED_DATA);
		}

	}
}

//块清屏
void oled_drawbmp_block_clear(int bx, int by, int clear_size)
{
	unsigned int i;
	//
	if (bx < 0)
	{
		bx = 0;
	}
	//
	OLED_SetPos(bx, by);
	for (i = 0; i < clear_size; i++)
	{
		if (bx + i>128) break;
		oledWriteByte(0x00,OLED_DATA);
		// oledWriteByte(0xff,OLED_DATA);

	}
}

//通用清残影
void OLED_shadowclear(int screen_x, int screen_y, signed char head_deep, short width, short height, int* last_screen_x, int* last_screen_y, signed char* last_deep, char mode)
{
	signed char deep_change;
	signed char real_xspeed;
	signed char real_yspeed;
	signed char abs_xspeed;
	char        Number_of_rows = height/8;
	int i;

	real_xspeed  = screen_x - *last_screen_x;
	real_yspeed  = screen_y - *last_screen_y;
	deep_change  = *last_deep - head_deep;
	abs_xspeed   = real_xspeed;


	if (abs_xspeed < 0)
	{
		abs_xspeed = -abs_xspeed;
	}
	
    if (real_yspeed == 0)
    {
        if (real_xspeed > 0)
        {
			for (i = 0; i < Number_of_rows; i++)
			{
				if (*last_deep + i >= 7) break;
				oled_drawbmp_block_clear(*last_screen_x, *last_deep + i, abs_xspeed);
			}
        }
        else if (real_xspeed < 0)
        {
			for (i = 0; i < Number_of_rows; i++)
			{
				if (*last_deep + i >= 7) break;
				// if (screen_x + width > 127)
				// {
				// 	/* code */
				// }
				
				oled_drawbmp_block_clear(screen_x + width, *last_deep + i, abs_xspeed);
			}
        }
    }
    else//y速度不为0
    {
        if (real_yspeed > 0 && head_deep + Number_of_rows < 8)//y速度 > 0
        {
			oled_drawbmp_block_clear(*last_screen_x, head_deep + Number_of_rows, width);
		}
		else if (real_yspeed < 0)
        {
			if (mode == hentai && head_deep - 2 >= 0)
			{
				if (!(head_deep - 2 > 7))
				oled_drawbmp_block_clear(*last_screen_x, head_deep - 2, width);
			}
			else if (mode == normalcy && head_deep - 1 >= 0)
			{
				if (!(head_deep - 1 > 7))
				oled_drawbmp_block_clear(*last_screen_x, head_deep - 1, width);
			}
		}
		
		if (real_xspeed > 0)
		{
			for (i = 0; i < Number_of_rows + 1; i++)
			{
				oled_drawbmp_block_clear(*last_screen_x, *last_deep - 1 + i, abs_xspeed);
			}
		}
		else if (real_xspeed < 0)
		{
			for (i = 0; i < Number_of_rows + 1; i++)
			{
				oled_drawbmp_block_clear(screen_x + width, *last_deep - 1 + i, abs_xspeed);
			}
		}
        
    }
	*last_deep = head_deep;
	*last_screen_x = screen_x;
	*last_screen_y = screen_y;
}

void y_invariantt_shadowclear(short x, short y, short width, short height)
{
	char row = height/8;
	char deep = 7 - y/8;
	for (Array_row = 0; Array_row < row; Array_row++)
	{
		if (dino_xspeed > 0)
		{
			oled_drawbmp_block_clear(x + width, deep + Array_row,abs(dino_xspeed));
		}
		else if(dino_xspeed < 0)
		{
			oled_drawbmp_block_clear(x + dino_xspeed, deep + Array_row,abs(dino_xspeed));
		}	
	}
}
