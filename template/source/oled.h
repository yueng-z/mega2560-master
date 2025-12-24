#include <iom2560v.h>
#include <macros.h>
#include "board.h"
#ifndef __OLED_H
#define __OLED_H			  	 

  

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define OLED_MODE 0

#define OLED_CS (1<<3)  //????
#define OLED_RST (1<<6)
#define OLED_DC  (1<<2) 
#define OLED_SCL (1<<1) 
#define OLED_SDIN (1<<2) 


#define OLED_CS_Clr()  PORTL&=~OLED_CS
#define OLED_CS_Set()  PORTL|=OLED_CS

#define OLED_RST_Clr() PORTL&=~OLED_RST
#define OLED_RST_Set() PORTL|=OLED_RST

#define OLED_DC_Clr() PORTG&=~OLED_DC
#define OLED_DC_Set() PORTG|=OLED_DC

#define OLED_SCLK_Clr() PORTB&=~OLED_SCL
#define OLED_SCLK_Set() PORTB|=OLED_SCL

#define OLED_SDIN_Clr() PORTB&=~OLED_SDIN
#define OLED_SDIN_Set() PORTB|=OLED_SDIN





//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	 
#define OLED_W   	X_WIDTH
#define OLED_H		Y_WIDTH		
#define GRAM_H 		OLED_H/8	  
//-----------------OLED???????????----------------  					   

//void delay_ms(unsigned int ms);


 		     

//OLED???????o????y
void oledWriteByte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void oledClear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void oledShowChar(u8 x,u8 y,u8 chr);

void oledShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2);

void oledShowString(u8 x,u8 y, u8 *p);	 
void oledSetPos(unsigned char x, unsigned char y);
void oledShowCHinese(u8 x,u8 y,u8 no); 
void OLED_DrawBMP(unsigned char xs, unsigned char ys, unsigned char xw, unsigned char yh, const unsigned char BMP[]);
#endif  
	 



