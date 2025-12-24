#ifndef __DINO_H
#define __DINO_H



extern const unsigned char  RESTART[];
extern const unsigned char  COVER[];


// 控制接入
void OLED_dino_control(char keyValue);

// 位置解算
void OLED_dino_pos();

// 绘制小恐龙
void OLED_DrawDino();

// 绘制跳跃小恐龙
int OLED_DrawDinoJump();

// 绘制重启
void OLED_DrawRestart();


int shit();

#endif




