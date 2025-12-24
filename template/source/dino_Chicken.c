#include "dino_Chicken.h"
#include "oled.h"
#include "dino_ShareData.h"
#include "dino_Sharemethod.h"
#include "dino_level.h"
#include "dino_Square.h"



#define   chicken_left       x
#define   chicken_right      x + 15 
#define   chicken_bottom     chicken[num][1]
#define   chicken_top        chicken[num][1] + 7

#define      edge_dinox_left         dino_x
#define      edge_dinox_right        dino_x+15
#define      edge_dinoy_head         dino_y+15
#define      edge_dinoy_foot         dino_y

// const short   		new_chicken[][6] = {//70x,0y,2speed,0碰撞，0上一次碰撞,0start_y，last上一次
//     70 ,0,2,0,0,0,
//     120,0,2,0,0,0,
// 	160,0,2,0,0,0,
// };

// short chicken_num = sizeof(new_chicken)/12;

// short   		chicken[][6] = {//70x,0y,2speed,0碰撞，0上一次碰撞,0start_y，last上一次
//     70 ,0,2,0,0,0,
//     120,0,2,0,0,0,
// 	160,0,2,0,0,0,
// };



short 			screen_chicken[9][4] ={

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

// int last_info[][4] = 
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

//左鸡
const unsigned char CHICKEN1[2][16] ={
// 0x00,0x00,0x08,0x08,0x1E,0x3B,0xBF,0xF9,0xA8,0xE4,0x34,0x3C,0x1E,0x0A,0x00,0x00,
0x00,0x00,0x08,0x08,0x1E,0x3B,0xBF,0xF9,0xA8,0xE4,0x34,0x3C,0x1C,0x0A,0x00,0x00,//不动鸡
// 0x00,0x00,0x08,0x88,0x9E,0x7B,0x3F,0x39,0x28,0x24,0x34,0xBC,0xDE,0x0A,0x00,0x00,
0x00,0x00,0x08,0x88,0x9E,0x7B,0x3F,0x39,0x28,0x24,0x34,0xBC,0xDC,0x0A,0x00,0x00,//动机
};
//右鸡
const unsigned char CHICKEN2[2][16] ={
0x00,0x00,0x0A,0xDC,0xBC,0x34,0x24,0x28,0x39,0x3F,0x7B,0x9E,0x88,0x08,0x00,0x00,//不动鸡
0x00,0x00,0x0A,0x1C,0x3C,0x34,0xE4,0xA8,0xF9,0xBF,0x3B,0x1E,0x08,0x08,0x00,0x00,//动机
};
//跳鸡
const unsigned char CHICKENJUMP[8][32] ={

0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x44,0x4F,0x3D,0x1F,0x1C,0x14,0x12,0x1A,0x5E,0x6E,0x05,0x00,0x00,

0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0x40,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x02,0x22,0x27,0x1E,0x0F,0x0E,0x0A,0x09,0x0D,0x2F,0x37,0x02,0x00,0x00,

0x00,0x00,0x00,0x00,0xC0,0x60,0xE0,0x20,0x00,0x80,0x80,0x80,0x80,0x40,0x00,0x00,0x00,0x00,0x01,0x11,0x13,0x0F,0x07,0x07,0x05,0x04,0x06,0x17,0x1B,0x01,0x00,0x00,

0x00,0x00,0x80,0x80,0xE0,0xB0,0xF0,0x90,0x80,0x40,0x40,0xC0,0xC0,0xA0,0x00,0x00,0x00,0x00,0x00,0x08,0x09,0x07,0x03,0x03,0x02,0x02,0x03,0x0B,0x0D,0x00,0x00,0x00,

0x00,0x00,0x40,0x40,0xF0,0xD8,0xF8,0xC8,0x40,0x20,0xA0,0xE0,0xD0,0x40,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x03,0x01,0x01,0x01,0x01,0x01,0x05,0x06,0x00,0x00,0x00,

0x00,0x00,0x20,0x20,0x78,0xEC,0xFC,0xE4,0xA0,0x90,0xD0,0xF0,0x68,0x20,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x00,0x00,0x00,

0x00,0x00,0x10,0x10,0x3C,0xF6,0x7E,0x72,0x50,0x48,0x68,0x78,0xB4,0x10,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,


0x00,0x00,0x08,0x88,0x9E,0x7B,0x3F,0x39,0x28,0x24,0x34,0xBC,0xDA,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};


//加载鸡
//加载鸡
char load_chicken() {
	// short chicken_num = sizeof(chicken)/12;
	short x,y,i;
    short chicken_sign=1;
    signed char chicken_state;
	Array_column = 0;
	//printf("chicken_num = %d ",chicken_num);
	for (i = 0; i < chicken_num; i++)
	{
		x = chicken[i][0];
		y = chicken[i][1];
        if(chicken[i][3] != 1)
        {
            chicken[i][0]=chicken[i][0]-chicken[i][2];
        }
        chicken_state = OLED_Square_chicken_map_Detect(&chicken[i][0],&chicken[i][1],16,8,-chicken[i][2],0,(short)last_xy[i][1],&Array_row);
        // OLED_Square_chicken_map_Detect(short *hero_x, short *hero_y, int width, int height, signed char hero_xspeed, signed char hero_yspeed, short last_hero_y,char *sign_of_jump)
        if (x + 15 >= screen_left - 13 && x <= screen_right + 13)//鸡在屏幕内
		{
			screen_chicken[Array_column][0] = x - screen_left;
			screen_chicken[Array_column][1] = y;
            
            // chicken_state = OLED_Square_Detect(&screen_chicken[i][0],&screen_chicken[i][1],16,8,-chicken[i][2],0);
            if(chicken_state == 0)
            {
                chicken[i][2] = -chicken[i][2];
            }
			screen_chicken[Array_column][2] = i;
            chicken_sign=2;
            if (x + 15 >= screen_left && x <= screen_right)/*鸡在屏幕内*/chicken_sign=1;
            screen_chicken[Array_column][3] = chicken_sign;
			Array_column++;
		
		}	
	}
}

//画鸡判断
void Can_I_Draw_Chicken()
{
	short x,y,num, row =0;
    short chicken_sign;
    // int *p1 = &(chicken[num][6]), *p2 = &(chicken[num][7]);
	// signed char* p3 = &(signed char)chicken[num][8];
	x = screen_chicken[temporary_num][0];
	y = screen_chicken[temporary_num][1];
	num = screen_chicken[temporary_num][2];//对应chicken第几只
    chicken_sign = screen_chicken[temporary_num][3];//鸡的画的判断
	if (num != -1)
	{       
        if(chicken_sign == 1){
            OLED_DrawChicken(x,y,num);
        }
        OLED_shadowclear(x,y,7-(chicken[num][1])/8,16, 8, &last_xy[num][0],&last_xy[num][1],&last_deep[num][0], hentai);
    
		temporary_num++;
		Can_I_Draw_Chicken();
	}
	else
	{
		temporary_num = 0;
	}

}

void OLED_DrawChickenDetect(short x, short y,short num) {
    if (dino_yspeed >= 0)
     {
        // 鸡向左走
        // if ((edge_dinox[1] >= edge_chickenx[0]) && (edge_dinox[1] <= edge_chickenx[1]) && (dino_y < edge_chickeny[2])) {
     
        if ((edge_dinox_right >= chicken_left) && (edge_dinox_right <= chicken_right) && (edge_dinoy_foot< chicken_top)) {
            // x = edge_dinox_right + 1;
            // chicken[num][2]= -chicken[num][2];
            dino_die = 1;
        } // 鸡向右走
        if ((edge_dinox_left <= chicken_right) && (edge_dinox_left >= chicken_left) && (edge_dinoy_foot< chicken_top)) {
            // x = edge_dinox_left - 16;
            // chicken[num][2]= -chicken[num][2];
            dino_die = 1;
    
        }
    } 
    else if (dino_yspeed < 0)
     {
        if ((edge_dinox_left <= chicken_right) && (edge_dinox_left >= chicken_left) && (edge_dinoy_foot > chicken_bottom) && (edge_dinoy_foot < chicken_top)) {
            // chicken_collided = 1;
            score+=100;
            chicken[num][2]= -chicken[num][2];
            chicken[num][3]= 1;
            UART1_send(cxk,6);
        }
        if ((edge_dinox_right >= chicken_left) && (edge_dinox_right <= chicken_right) && (edge_dinoy_foot >  chicken_bottom) && (edge_dinoy_foot < chicken_top)) {
            // chicken_collided = 1;
            score+=100;
            chicken[num][2]= -chicken[num][2];
             chicken[num][3]= 1;
            UART1_send(cxk,6);
        }
    }
	// printf(" dinox=%d, chickenx=%d      ",edge_dinox[1],x);
    //  printf("num=%d",num);
}

char OLED_DrawChicken(short x,short y,short num) {
    int chicken_deep = 0;
    // static char dir = 0;
    char setx,draw_start_x;
    static char  dir[]={0}  ;
    unsigned char byte;
    signed char offset = 0; // 小鸡脚部所在行
    int width = sizeof(CHICKEN1) / 2;
   
    if(chicken[num][4] == 3) return 0;//小鸡死亡
    if (chicken[num][3] != 1)
    {
        OLED_DrawChickenDetect(x,y,num);
    }
    if (chicken[num][3] == 1 && chicken[num][4] == 0) {   //碰撞后将chicken[num][4]记录为1
        chicken[num][4] = 1;
        chicken[num][5] = chicken[num][1];
        OLED_DrawJumpChicken(x,y,num);
    }
    else if (chicken[num][3] == 1 && chicken[num][4] != 3) {//碰撞且chicken[num][4]不是死亡状态时画鸡
        OLED_DrawJumpChicken(x,y,num);
    }
    else
    {
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
            // x -= chicken[num][2];
        // chicken[num][0] -=  chicken[num][2];
        OLED_SetPos(setx,7-(chicken[num][1])/8);
        //printf("x = %d", x);
        for (Array_column = draw_start_x; Array_column< width;Array_column++)
        {
            if (chicken[num][0] - screen_left +Array_column > 127) break;
            if (chicken[num][2] > 0 ) 
            {
                byte = CHICKEN1[dir[num]][Array_column];
            }
            else  if (chicken[num][2] < 0 ) 
            {
                byte = CHICKEN2[dir[num]][Array_column];
            }
            oledWriteByte(byte, OLED_DATA);
        }
        
        
    }
    if (chicken[num][1] < -20)//y小于-20时死亡，
    {
        chicken[num][4] = 3;
    }

    dir[num]++;
    dir[num] = dir[num] % 2;
}

const char jump_v = 7; // 跳跃速度
const int jump_top = 31; // 跳跃的最高点
//跳跃鸡
void OLED_DrawJumpChicken(short x,short y,short num) {
    int chicken_deep = 0;
    // int chicken_x = 0;
    static char dir = 0;
    unsigned char byte;
    signed char offset = 0; // 小鸡脚部所在行
    int width = sizeof(CHICKEN1) / 2;
    signed char jump_height = 0; // 跳跃高度
    char j;

    if (chicken[num][4] == 2) //下落
    {
        chicken[num][1] -= jump_v;
        jump_height= chicken[num][1]-chicken[num][5];
        if(jump_height < 0)
        {
            jump_height = 7+(jump_height + 1)%8;
        }
        // if (jump_height <=0) { 
        //     CHICKEN_DISAPPEARED =1;    // }
    } 
    else if (chicken[num][3] == 1 || chicken[num][4] == 1)  {//上升（发生碰撞，，chicken[num][4]已经碰过一次）
        // jump_height += jump_v;
        chicken[num][1] += jump_v;
        jump_height= chicken[num][1]-chicken[num][5];
        
        if (jump_height >= jump_top) {
            jump_height = jump_top;
            chicken[num][1] = chicken[num][5] + jump_top;
            chicken[num][4] = 2;
            // CHICKEN_FALLING =1;
            // jump_v = -7; // 下落速度
        }
    } 
    // printf("chicken[num][1] = %d,jump_height = %d",chicken[num][1],jump_height);
    if (chicken[num][1] < -7) offset = -2;
    else if (chicken[num][1] < 0) offset = -1;
    else if (chicken[num][1] <= 7) offset = 0;
    else if (chicken[num][1] <= 15) offset = 1;
    else if (chicken[num][1] <= 23) offset = 2;
    else if (chicken[num][1] <= 31) offset = 3;
    else if (chicken[num][1] <= 39) offset = 4;
    else if (chicken[num][1] <= 47) offset = 5;
    else if (chicken[num][1] <= 55) offset = 6;
    else if (chicken[num][1] <= 63) offset = 7;
    // printf("offset = %d, co = %d, state = %d, jump_height = %d    ", offset, chicken_collided, chicken_state, jump_height);

    for (Array_row= 0; Array_row <2;Array_row++)
    {
        if(6-offset + Array_row < 0) continue;
        if(6-offset + Array_row > 7) break;
        OLED_SetPos(x,6-offset + Array_row);
        for (Array_column = 0; Array_column <width; Array_column++) 
        {
            j = width*Array_row + Array_column;
            byte = CHICKENJUMP[jump_height % 8][j];
            oledWriteByte(byte, OLED_DATA);
        }
    }

   
}

void Chicken()
{
	temporary_num = 0;
    memset(screen_chicken, -1, sizeof(screen_chicken));
	load_chicken();	
	Can_I_Draw_Chicken();
    //printf("chicken1 = %d, chicken1 = %d,chicken1 = %d   |||",chicken[0][3],chicken[1][3],chicken[2][3]);
	
}
