#include "dino_ShareData.h"

//speed
signed char         dino_xspeed = 0;
signed char         dino_yspeed = 0;
char                direction = '->';
short        		x_sta = 0;
short        		y_sta = 0;


//position
signed int          pos_on_map = begin_pos_on_map;
short               dino_x = 0;
// short               dino_y = 0;
short*              P_dino_x = &dino_x; 
short               dino_y = 0;
short               last_dino_y = 0;
unsigned int        dino_pos_state;
signed char         dino_deep = 6;//小恐龙头部所在行



//state
char                foothold = 1;
char				*p_foothold = &foothold;
char                jumping = 0;
signed int        	jump_start_y;
char 				control = 0;
char                Rise = 1;
char				castle_sign = 0;
char				cover = 1;
char				dino_die = 0;
char				finish = 0;
signed char         choose = 0;

//data
short 				temporary_num = 0;//公用计时器，用完要归零
char 				temporary_num1 = 0;
char 				temporary_num2 = 0;
char				life = 3;
char				Array_row;
char 				Array_column;
char                level = 1;
unsigned int        score = 0;
// unsigned int        last_score = 0;
unsigned int        Draw_num = 0;
unsigned int        max_score = 0;
unsigned int        SEG7_number = 10000;
unsigned char       SEG7_output[4] = {0,0,0,0};

char mla_volume[] = {0xAA, 0x13, 0x01, 0x0F, 0xCD}; 

char mla_bgm[]={0xAA, 0x1B, 0x02, 0x30, 0x31, 0x28};
char mla_1up[]={0xAA, 0x1B, 0x02, 0x30, 0x32, 0x29};
char mla_jump[]={0xAA, 0x1B, 0x02, 0x30, 0x33, 0x2A};
char mla_coin[]={0xAA, 0x1B, 0x02, 0x30, 0x34, 0x2B};
char mla_bump[]={0xAA, 0x1B, 0x02, 0x30, 0x35, 0x2C};
char mla_die[]={0xAA, 0x1B, 0x02, 0x30, 0x36, 0x2D};
char mla_finish[]={0xAA, 0x1B, 0x02, 0x30, 0x37, 0x2E};
char js[] = {0xAA, 0x1B, 0x02, 0x30, 0x38, 0x2F};
char cxk[] = {0xAA, 0x1B, 0x02, 0x30, 0x39, 0x30};
// char yl[]={0xAA, 0x13, 0x01, 0x14, 0xD2};


