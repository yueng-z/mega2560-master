// 加载方块
void load_square();

// 绘制方块
void OLED_DrawSquare();

//碰撞检测
int OLED_Square_Detect(short *hero_x,  short *hero_y,int width, int height, signed char hero_xspeed, signed char hero_yspeed);

int OLED_Square_chicken_map_Detect(short *hero_x, short *hero_y, int width, int height, signed char hero_xspeed, signed char hero_yspeed, short last_hero_y,char *sign_of_jump);