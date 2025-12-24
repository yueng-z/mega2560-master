#include "dino_Square.h"
#include "dino_ShareData.h"
#include "dino_Sharemethod.h"
#include "dino_level.h"
#include "oled.h"
#include "dino.h"


//detect
signed short square_y_top = 0;
signed short square_y_tail = 0;



// const short   square[][2] = 
// {
//     // 47,5,
//     47,7,
//     125,7,
//     300,7,
// 	317,7,
//     450,7,
//     500,5,
//     30,5,
//     200,7,
// 	217,7,
// };

short screen_square[9][3] =
{
	-1,-1,-1,
	-1,-1,-1,
	-1,-1,-1,
	-1,-1,-1,
	-1,-1,-1,
	-1,-1,-1,
	-1,-1,-1,
	-1,-1,-1,
	-1,-1,-1,
};


// int last_square_info[][4] = 
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

const unsigned char  SQUARE[] =
{
	// 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
   0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xDB,0xDB,0xDB,0xDB,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF
};

//加载方块
void load_square() {
	// short square_num = sizeof(square)/4;
	short x, i;
	unsigned char height;
	short Array_row = 0;
	// printf("square_num = %d ",square_num);
	for (i = 0; i < square_num; i++)
	{
		x = square[i][0];
		height = square[i][1];
		if (x + 16 - 1 >= screen_left - 10 && x <= screen_right + 10)
		{
			screen_square[Array_row][0] = x - screen_left;
			screen_square[Array_row][1] = height;
			screen_square[Array_row][2] = i;
			// printf("x = %d", screen_square[Array_row][0]);
			Array_row++;
		}	
	}	
}


// 绘制方块
void OLED_DrawSquare()
{
	int x;
	short i;
	char deep=0;	
	static char width=0;
	int start_x = 0;
	char speed = 5;
	unsigned char byte;
    unsigned int j = 0;
	char num;
	// int* p1;
	// int* p2;
	// signed char* p3;

	// int* same_p1;
	// int* same_p2;
	// signed char* same_p3;

	int detect_state;
	// static int last_screen_x;
	// static int last_screen_y;
	// static signed char last_deep;

    char setx,draw_start_x;

	load_square();

	for (i = 0; i < 9; i++) 
	{
		// if(screen_square[i][0] + 15 >= 0 && screen_square[i][0] <= 127 && screen_square[i][1] != -1)
		if(screen_square[i][1] != -1)
		{	
			square_y_tail = 8 * (7 - screen_square[i][1]);

			// for (Array_row = 0; Array_row < 9; Array_row++) 
			// {
			// 	if(last_square_info[Array_row][3] == screen_square[i][2])
			// 	{
			// 		num = Array_row;
			// 		break;
			// 	}
			// }
			// p1 = &last_square_info[Array_row][0];
			// p2 = &last_square_info[Array_row][1];
			// p3 = (signed char*)&last_square_info[Array_row][2];

            // same_p1 = p1;
            // same_p2 = p2;
            // same_p3 = p3;



            // printf("screen_square[%d][0] = %d       ",i,screen_square[i][0]);
            
            // if (square[i][0] <= screen_right && square[i][0] + 15 >= screen_left)
            if (screen_square[i][0] <= 127 && screen_square[i][0] + 15 >= 0)
            {

                if (screen_square[i][0]< 0)
                {
                    setx = 0;
                    draw_start_x = -(screen_square[i][0]);
                }
                else
                {
                    setx = screen_square[i][0];
                    draw_start_x = 0;
                }
                for (Array_row = 0; Array_row < 1; Array_row++)
                {
                    OLED_SetPos(setx, screen_square[i][1]);
                    for (Array_column = draw_start_x; Array_column < 16; Array_column++)
                    {
                        if (screen_square[i][0] + Array_column > 127) break;
                        j = Array_row*16 + Array_column;
                        oledWriteByte(SQUARE[j],OLED_DATA);
                    }
                }
                
                // OLED_shadowclear(screen_square[i][0], square_y_tail, screen_square[i][1], 16, 8, p1, p2, p3,hentai);	     
                // OLED_shadowclear(screen_square[i][0], square_y_tail, screen_square[i][1], 16, 8, same_p1, same_p2, same_p3,hentai);	
                y_invariantt_shadowclear(screen_square[i][0], square_y_tail, 16,8);
            }
            // else if (square[i][0] <= screen_right + 16 && square[i][0]  + 15 >= screen_left - 16)
            else if (screen_square[i][0] <= 127 + 10 && screen_square[i][0]  + 15 >=  - 10)
            {

                // OLED_shadowclear(screen_square[i][0], square_y_tail, screen_square[i][1], 16, 8, p1, p2, p3,hentai);
                // OLED_shadowclear(screen_square[i][0], square_y_tail, screen_square[i][1], 16, 8, same_p1, same_p2, same_p3,hentai);
                y_invariantt_shadowclear(screen_square[i][0], square_y_tail, 16,8);
            }


			// last_square_info[i][0] = screen_square[i][0];
			// last_square_info[i][1] = square_y_tail;
			// last_square_info[i][2] = screen_square[i][1];
			// last_square_info[i][3] = screen_square[i][2];
		}
		
	}
    // detect_state = OLED_Square_Detect(&dino_x, &dino_y, 16, 16, dino_xspeed, dino_yspeed);
    OLED_Square_Screen_Detect(&dino_x, &dino_y, 16, 16, dino_xspeed, dino_yspeed, last_dino_y,&jumping);
    // if(detect_state == 2)
    // {
    //     score+=50;
    // }
	memset(screen_square,-1,sizeof(screen_square));
}


int OLED_Square_Detect(short *hero_x, short *hero_y, int width, int height, signed char hero_xspeed, signed char hero_yspeed)  
{  
    int detect_state = -1;  
    short i;  
    short Array_row = 0;  
    for (i = 0; i < square_num; i++)  
    {  
        short square_y_top = 8 * (7 - square[i][1]) + 7;  
        short square_y_tail = 8 * (7 - square[i][1]);  

        if ((((*hero_x + width - 1 + screen_left) >= square[i][0]) && ((*hero_x + width - 1 + screen_left) <= square[i][0] + 15) && (hero_xspeed >= 0)) ||  
            ((*hero_x + screen_left >= square[i][0]) && (*hero_x + screen_left <= square[i][0] + 15) && (hero_xspeed <= 0)))  
        {  

            if (hero_yspeed == 0 && *hero_y <= square_y_top && (*hero_y + height - 1) >= square_y_tail)  
            {  
                detect_state = 0;  
                // 在方块左右边缘  
                if (hero_xspeed > 0)  
                {  
                    *hero_x = square[i][0] - 16 - screen_left;
                }  
                else if (hero_xspeed < 0)  
                {  
                    *hero_x = square[i][0] + 16 - screen_left;
                }  
            }  

            if (hero_yspeed <= 0 && *hero_y - 1 <= square_y_top && *hero_y > square_y_tail)  
            {  
                detect_state = 1;  
                // 跳到上面  
                foothold = 1;  
                *hero_y = square_y_top + 1;  
            }  

            if (hero_yspeed >= 0 && (*hero_y + height - 1) < square_y_top && (*hero_y + height - 1) >= square_y_tail)  
            {  
                detect_state = 2;  
                // 头撞到方块  
                jumping = 0;  
            }  

            return detect_state;  
        }  
         
    }  
}

void OLED_Square_Screen_Detect(short *hero_x, short *hero_y, int width, int height, signed char hero_xspeed, signed char hero_yspeed, short last_hero_y,char *sign_of_jump)  
{  
    int detect_state = -1;  
    short i;  
    short Array_row = 0;   
    for (i = 0; i < 9; i++)  
    {  
        short square_y_top = 8 * (7 - screen_square[i][1]) + 7;  
        short square_y_tail = 8 * (7 - screen_square[i][1]);  
        if (screen_square[i][2] == -1) break;
        if (square_y_top >= *hero_y && square_y_tail <= *hero_y + 15)
        {
            if(*hero_x + width - 1 >= screen_square[i][0] && *hero_x <= screen_square[i][0])
            {
                if (last_hero_y > square_y_top)
                {
                    // detect_state = 1;  
                    *hero_y = square_y_top + 1;
                    foothold = 1;
                }
                else if(last_hero_y + 15 < square_y_tail)
                {
                    // detect_state = 2;
                    *sign_of_jump = 0;
                    *hero_y = square_y_tail - 16;
                    UART1_send(mla_bump,6);
                }
                else 
                {
                    detect_state = 0;
                    *hero_x = screen_square[i][0] - 16;
                    UART1_send(mla_bump,6);
                }
                
            }
            if(*hero_x + width - 1 >= screen_square[i][0] + 15 && *hero_x <= screen_square[i][0] + 15)
            {
                if (last_hero_y > square_y_top)
                {
                    // detect_state = 1;  
                    *hero_y = square_y_top + 1;
                    foothold = 1;
                }
                else if(last_hero_y + 15 < square_y_tail)
                {
                    // detect_state = 2;
                    *sign_of_jump = 0;
                    UART1_send(mla_bump,6);
                }
                else 
                {
                    detect_state = 0;
                    *hero_x = screen_square[i][0] + 16;
                    UART1_send(mla_bump,6);
                }
            }
        }
        // printf("%d detect_state = %d ,last_hero_y = %d ||",i,detect_state,last_hero_y);
    }
    
}

int OLED_Square_chicken_map_Detect(short *hero_x, short *hero_y, int width, int height, signed char hero_xspeed, signed char hero_yspeed, short last_hero_y,char *sign_of_jump)  
{  
    
    short i;  
    short Array_row = 0;
    signed char detect_state = -1;
    for (i = 0; i < square_num; i++)  
    {  
        short square_y_top = 8 * (7 - square[i][1]) + 7;  
        short square_y_tail = 8 * (7 - square[i][1]);  

        if (square_y_top >= *hero_y && square_y_tail <= *hero_y + 15)
        {
            if(*hero_x + width - 1 >= square[i][0] && *hero_x <= square[i][0])
            {
                if (last_hero_y > square_y_top)
                {
                    detect_state = 1;  
                    *hero_y = square_y_top + 1;
                    foothold = 1;
                    return detect_state;
                }
                else if(last_hero_y + 15 < square_y_tail)
                {
                    detect_state = 2;
                    *sign_of_jump = 0;
                    *hero_y = square_y_tail - 16;
                    // UART1_send(mla_bump,6);
                    return detect_state;
                }
                else 
                {   detect_state = 0;
                    *hero_x = square[i][0] - 16;
                    // UART1_send(mla_bump,6);
                    return detect_state;
                }
                
            }
            if(*hero_x + width - 1 >= square[i][0] + 15 && *hero_x <= square[i][0] + 15)
            {
                if (last_hero_y > square_y_top)
                {
                    detect_state = 1;  
                    *hero_y = square_y_top + 1;
                    foothold = 1;
                    return detect_state;
                }
                else if(last_hero_y + 15 < square_y_tail)
                {
                    detect_state = 2;
                    *sign_of_jump = 0;
                    // UART1_send(mla_bump,6);
                    return detect_state;
                }
                else 
                {
                    detect_state = 0;
                    *hero_x = square[i][0] + 16;
                    // UART1_send(mla_bump,6);
                    return detect_state;
                }
            }
        }
        // printf("%d detect_state = %d ,last_hero_y = %d ||",i,detect_state,last_hero_y);
    } 
}