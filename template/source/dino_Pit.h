extern short 				screen_pit[9][2];

//加载坑洞
void load_pit();

//画坑判断
char Can_I_Draw_Pit(char x,short *Draw_length);

//坠落判断
char drop_in_pit(short *screen_x, short screen_y, char width, char* sign_of_stand,char *P_in_pit);

// 显示坑
int OLED_DrawPit(char *p_drawmode,short Draw_length,char x);
