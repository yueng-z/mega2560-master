#include "dino_Pit.h"
#include "dino_ShareData.h"
#include "dino_level.h"
#include "oled.h"


short 				screen_pit[9][2] ={
	-1,-1,
	-1,-1,
	-1,-1,
	-1,-1,
	-1,-1,
	-1,-1,
	-1,-1,
	-1,-1,
	-1,-1,
};


//加载坑洞
void load_pit() {

	short x, i;
	unsigned char width;
	short Array_row = 0;
	//printf("pit_num = %d ",pit_num);
	for (i = 0; i < pit_num; i++)
	{
		x = pit[i][0];
		width = pit[i][1];
		if (x + width - 1 >= screen_left && x <= screen_right)
		{
			screen_pit[Array_row][0] = x - screen_left;
			screen_pit[Array_row][1] = width;
			//printf("x = %d", screen_pit[Array_row][0]);
			Array_row++;
			
		}	
		
	}
}

//画坑判断
char Can_I_Draw_Pit(char x,short *Draw_length)
{
	signed char drawmode = 0;

	if (screen_pit[temporary_num][1] != -1)
	{
		if (x > screen_pit[temporary_num][0] && x < screen_pit[temporary_num][0] + screen_pit[temporary_num][1] - 1)
		{
			*Draw_length = screen_pit[temporary_num][0] + screen_pit[temporary_num][1] - x;
			temporary_num = 0;
			return 2;
		} 
		else if (screen_pit[temporary_num][0] == x)
		{
			*Draw_length = screen_pit[temporary_num][0] + screen_pit[temporary_num][1] - x;
			temporary_num = 0;
			return 1;
		}
		else if (screen_pit[temporary_num][0] + screen_pit[temporary_num][1] - 1 == x) 
		{
			*Draw_length = 1;
			temporary_num = 0;
		 	return 1;
		}
		else
		{
		//printf("temporary_num = %d       ",temporary_num);
			temporary_num++;
			drawmode = Can_I_Draw_Pit(x,Draw_length);
		}
		
	}
	else
	{
		temporary_num = 0;
	}
	return drawmode;
}

//坠落判断
char drop_in_pit(short* screen_x, short screen_y, char width, char* sign_of_stand,char *P_in_pit)
{
	char drop = 0;
	if (screen_pit[temporary_num][1] != -1)
	{
		//printf("(temporary_num = %d , screen_pit[temporary_num][0] = %d , *screen_x = %d , screen_xright = %d , screen_pit[temporary_num][0]_right = %d )",temporary_num, screen_pit[temporary_num][0],*screen_x,*screen_x + width - 1,screen_pit[temporary_num][0] + screen_pit[temporary_num][1] - 1);

		if(*P_in_pit)
		{
			if (*screen_x + width - 1 > screen_pit[temporary_num][0] && *screen_x < screen_pit[temporary_num][0])
			{
				*screen_x = screen_pit[temporary_num][0] + 1; 
				//printf("1");
			}
			else if(*screen_x + width - 1 > screen_pit[temporary_num][0] + screen_pit[temporary_num][1] - 1 && *screen_x < screen_pit[temporary_num][0] + screen_pit[temporary_num][1] - 1)
			{
				*screen_x = screen_pit[temporary_num][0] + screen_pit[temporary_num][1] - 17;
				//printf("2");
			}
		}
		if (*screen_x > screen_pit[temporary_num][0] && *screen_x + width - 1 < screen_pit[temporary_num][0] + screen_pit[temporary_num][1] - 1 && screen_y <= 0) 
		{
			*sign_of_stand = 0;
			temporary_num = 0;
			return 1;
		}
		else
		{
			temporary_num++;
			drop = drop_in_pit(screen_x, screen_y, width, sign_of_stand,P_in_pit);
		}
		


	}
	else
	{
		temporary_num = 0;
		return drop;
	}
}

//绘制坑洞
int OLED_DrawPit(char *p_drawmode,short Draw_length,char x){

		if(Draw_length == 1)
		{
			*p_drawmode = 1;
		}
		if (*p_drawmode == 1)
		{
			oledWriteByte(0xf0,OLED_DATA);
			(*p_drawmode)++;
		}
		else if (*p_drawmode == 2)
		{
			oledWriteByte(0x00,OLED_DATA);
		}
}
