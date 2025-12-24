// //坐标设置
void OLED_SetPos(unsigned char x, unsigned char y);

// 快速绘制图像
void OLED_DrawBMPFast(const unsigned char BMP[]);

//块清屏
void oled_drawbmp_block_clear(int bx, int by, int clear_size);

//通用清残影
void OLED_shadowclear(int screen_x, int screen_y, signed char head_deep, short width, short height, int* last_screen_x, int* last_screen_y, signed char* last_deep, char mode);

//通用y不变清残影
void y_invariantt_shadowclear(short x, short y, short width, short height);

