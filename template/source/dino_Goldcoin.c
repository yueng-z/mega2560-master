#include "dino_Goldcoin.h"
#include "oled.h"
#include "dino_ShareData.h"
#include "dino_Sharemethod.h"
#include "dino_level.h"

char c_dir = 0;

// const unsigned char  cliff = 0xf0;

const unsigned char  GOLDCOIN[][6]={
	// 0x00,0x1C,0x22,0x5D,0x5D,0x22,0x1C,0x00,
	// 0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00,
	0x1C,0x22,0x5D,0x5D,0x22,0x1C,
	0x00,0x00,0x7F,0x7F,0x00,0x00,
};

// short   		goldcoin[][3] = {//x,y,碰撞，上一次碰撞
//     50,8,1,
//     90,8,1,
// 	100,8,1,
// 	150,8,1,
//     190,8,1,
// 	200,8,1,
// };

short 				screen_goldcoin[9][4] ={

	-1,-1,-1,0,
	-1,-1,-1,0,
	-1,-1,-1,0,
	-1,-1,-1,0,
	-1,-1,-1,0,
	-1,-1,-1,0,
	-1,-1,-1,0,
	-1,-1,-1,0,
	-1,-1,-1,0,
};

// short coin_last_info[][4] = 
// {
// 	-1,-1,-1,-1,
// 	-1,-1,-1,-1,
// 	-1,-1,-1,-1,
// 	-1,-1,-1,-1,
// 	-1,-1,-1,-1,
// 	-1,-1,-1,-1,
// 	-1,-1,-1,-1,
// 	-1,-1,-1,-1,
// 	-1,-1,-1,-1,
// };

void OLED_DrawGoldCoinDetect(short x, short y,short num)
{
	// if (((edge_dinox_left <= coin_right) && (edge_dinox_left >= coin_left) && (coin_top > dino_y) && (coin_bottom < dino_y + 15) )||((edge_dinox_right >= coin_left) && (edge_dinox_right <= coin_right)&& (coin_bottom <= dino_y + 15) && (coin_top >= dino_y) ))
	if((coin_top > dino_y) && (coin_bottom < dino_y + 15)&&(((edge_dinox_left <= coin_right) && (edge_dinox_left >= coin_left))||((edge_dinox_right >= coin_left) && (edge_dinox_right <= coin_right))))
    {
		score+=100;
		UART1_send(mla_coin,6);
		goldcoin[num][2] = 0;
	}
}
	

void OLED_DrawGoldCoin(short x,short y,short num)
{
   
    unsigned char byte;
	char setx,draw_start_x;
	unsigned int j = 0;
	int width = 6 ;

    OLED_DrawGoldCoinDetect(x,y,num);

	if (x< 0)
	{
		setx = 0;
		draw_start_x = -x;
	}
	else
	{
		setx = x;
		draw_start_x = 0;
	}
	OLED_SetPos(setx, 7-(goldcoin[num][1])/8);
	for (Array_row = draw_start_x; Array_row < width; Array_row++)
	{
	
		if (x  +Array_row > 127) break;
		j=Array_row;
		if(goldcoin[num][2] == 0)
		{
			oledWriteByte(0x0,OLED_DATA);
		}
		else{
			oledWriteByte(GOLDCOIN[c_dir][j],OLED_DATA);
		}
		
	}


}


//加载金币
void load_goldcoin()
{
	
	// short coin_num = sizeof(goldcoin)/6;
	short x, i;
	short coin_sign=1;
	unsigned char height;
	short Array_row = 0;
	for (i = 0; i <coin_num; i++)
	{
		if(goldcoin[i][2])
		{
			x = goldcoin[i][0];
			height = goldcoin[i][1];
			if (x + 7 >= screen_left - 10 && x <= screen_right + 10)
			{
				screen_goldcoin[Array_row][0] = x - screen_left;
				screen_goldcoin[Array_row][1] = height;
				screen_goldcoin[Array_row][2] = i;
				coin_sign = 2;
                if (x + 7 >= screen_left && x <= screen_right) coin_sign = 1;
             	screen_goldcoin[Array_row][3] = coin_sign;
				//printf("x = %d", screen_coin[Array_row][0]);
                // printf("goldcoin[i][0] = %d ,x = %d, screen_left = %d     ",goldcoin[i][0],x,screen_left);
				Array_row++;
			}
		}
	}
}

//画金币判断
void Can_I_Draw_GoldCoin()
{
	short x,y,num,row = 0;
	short coin_sign;
	// int *p1,*p2;
	// signed char* p3;
	// int  same_p1,same_p2;
	// signed char  same_p3;

	x   = screen_goldcoin[temporary_num][0];
	y   = screen_goldcoin[temporary_num][1];
	num = screen_goldcoin[temporary_num][2];
	coin_sign = screen_goldcoin[temporary_num][3];//硬币的画的判断
	
	if (num != -1)
	{
		// for (Array_row = 0; Array_row < 9; Array_row++) 
        // {
        //     if(coin_last_info[Array_row][3] == num)
        //     {
        //         row = Array_row;
        //         break;
        //     }
        // }
		// same_p1 = coin_last_info[row][0]; //上一次的
        // same_p2 = coin_last_info[row][1];
        // same_p3 = (signed char)coin_last_info[row][2];
        // p1 = &same_p1;
        // p2 = &same_p2;
        // p3 = &same_p3;

	    if(coin_sign == 1){
           	OLED_DrawGoldCoin(x,y,num);
			// OLED_shadowclear(x,y,7-(goldcoin[num][1])/8, 6, 8, p1, p2, p3, hentai);
        }
		// else {
		// 		OLED_shadowclear(x,y,7-(goldcoin[num][1])/8, 6, 8, p1, p2, p3, hentai);
        // }
		y_invariantt_shadowclear(x,y,6,8);
		//OLED_shadowclear(x,y,7-(goldcoin[num][1])/8, 6, 8, p1, p2, p3, hentai);
		// coin_last_info[temporary_num][0] = x;
		// coin_last_info[temporary_num][1] = y;
		// coin_last_info[temporary_num][2] = 7-(goldcoin[num][1])/8;
		// coin_last_info[temporary_num][3] = num;
		// printf("coin_sign = %d       ",coin_sign );
		temporary_num++;
		Can_I_Draw_GoldCoin();
	}
	else
	{
		temporary_num = 0;
	}

}

void COIN()
{
	temporary_num = 0;
	load_goldcoin();	
	Can_I_Draw_GoldCoin();
	c_dir++;
	c_dir = c_dir % 2;
	memset(screen_goldcoin, -1, sizeof(screen_goldcoin));
}