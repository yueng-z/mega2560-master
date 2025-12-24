// ICC-AVR application builder : 2022/5/16 22:59:09
//  Target : M2560
//  Crystal: 16.000Mhz

#include <iom2560v.h>
#include <macros.h>
#include <stdio.h>
#include "timer.h"
#include "board.h"
#include "i2c.h"
#include "key.h"
#include "dht11.h"
#include "gui.h"
#include "lcdtest.h"
#include "queue.h"
#include "esp32.h"
#include "oled.h"

#include "Dino_Game.h"
extern unsigned char            SEG7_output[4];
extern char                     mla_volume[];
extern char                     js[];

extern Queue q;

#define seg595_OE_port PA
#define seg595_OE_pin 6
#define seg595_ST_port PA
#define seg595_ST_pin 4

#define dz595_OE_port PA
#define dz595_OE_pin 7
#define dz595_ST_port PA
#define dz595_ST_pin 5

#define disp_EN_port PC
#define disp_EN_pin  7



unsigned char seg7Code[16] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6};

unsigned char Disp_data[8] = {2, 0, 2, 1, 1, 9, 9, 9};

const unsigned char zdhz[][32] = {
{0x00,0x0C,0x18,0x18,0x00,0x00,0xFF,0x18,0x18,0x18,0x18,0x18,0x78,0x38,0x18,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0xFF,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06},/*"�?",0*/
{0x30,0xF8,0x1F,0x18,0x18,0xFF,0x18,0x3C,0x7C,0x1E,0xDE,0x1B,0x18,0x18,0x18,0x18,0x18,0x19,0x1B,0x1B,0x18,0x19,0x1B,0x1B,0x18,0xF8,0x1F,0x18,0x18,0x18,0x18,0x18}/*"�?",1*/
};
uint8_t dzBuffer[][32];
U16 ledTimer;

void userInit(void)
{
  //For 7seg display
	gpioWrite(dz595_OE_port, dz595_OE_pin, HIGH); // disable the DZ595 output	
  //For DdianZzhen(DZ) display	
	// gpioWrite(dz595_OE_port, seg595_OE_pin, LOW);// disable the 7SEG output
	// gpioWrite(disp_EN_port, disp_EN_pin, HIGH);
	
	//gpioWrite(PL, 1, HIGH); // Enable ESP32
}
#define SET_DS()  		gpioWrite(PB,2,HIGH)
#define CLR_DS()  		gpioWrite(PB,2,LOW)
#define SHCLK()	  		gpioWrite(PB,1,LOW);  gpioWrite(PB,1,HIGH)
#define SEG_ST_CLK()	gpioWrite(PA,4,LOW);  gpioWrite(PA,4,HIGH)

void hc595Send(uint8_t data)
{
	int posit;
	for(posit=0; posit<8; posit++)
	{
		if (data&BIT(7-posit)) {
      		SET_DS();
    	} else {
      		CLR_DS();		
    	}
		SHCLK();
	}
	//  
}
void segSelect(char index)
{
	PORTA &= 0xf0;
	PORTA |= index;
}
int segIndex, dzIndex, rollIndex = 6;
/*
	spitransferByte(zdhz[3][ dzIndex + 16]);
	spitransferByte(zdhz[3][ dzIndex]);
	spitransferByte(zdhz[2][ dzIndex + 16]);
	spitransferByte(zdhz[2][ dzIndex]);

*/
	
void dzDisplay(void)
{
    const char *p = &zdhz[0][0];
	//gpioWrite(dz595_OE_port, dz595_OE_pin, HIGH);
	hc595Send(p[rollIndex*16+ dzIndex + 16]);
	hc595Send(p[rollIndex*16+ dzIndex]);
	hc595Send(p[rollIndex*16-32+dzIndex + 16]);
	hc595Send(p[rollIndex*16-32+dzIndex]);

	SEG_ST_CLK();  //ST clk to store the serial data
	segSelect(dzIndex);
	//gpioWrite(dz595_OE_port, dz595_OE_pin, LOW);
	dzIndex++;
	if (dzIndex > 15)
		dzIndex = 0;
}

// void segDisplay(void)
// {
// 	gpioWrite(seg595_OE_port, seg595_OE_pin, HIGH); //????
// 	hc595Send(seg7Code[Disp_data[segIndex]]);
// 	SEG_ST_CLK();  
// 	segSelect(segIndex);
// 	gpioWrite(seg595_OE_port, seg595_OE_pin, LOW);  //????
// 	segIndex++;
// 	if (segIndex > 7)
// 		segIndex = 0;
// }
void segDisplay(void)
{
	gpioWrite(seg595_OE_port, seg595_OE_pin, HIGH);
	// spitransferByte(seg7Code[Disp_data[segIndex]]);
    spitransferByte(seg7Code[SEG7_output[segIndex]]);
	gpioWrite(seg595_ST_port, seg595_ST_pin, LOW);
	gpioWrite(seg595_ST_port, seg595_ST_pin, HIGH);
	segSelect(segIndex);
	gpioWrite(seg595_OE_port, seg595_OE_pin, LOW);
	segIndex++;
	if (segIndex > 3)
		segIndex = 0;
}


int counter;
char smdbs = 0;
void interrupt_1ms(void)
{
    if(smdbs) 
    {segDisplay();}
}

int main(void)
{
    //
    int i = 0;
    // int j=0;
    // unsigned int score = 0;
	// int height = 0;
    // char reset = 0;

	uint8_t temp, humi;
	// uint8_t keyValue;
    char keyValue;
	char sendBuf[10];
	boardInit();
	userInit();

#if USINGLCD
    
	LCD_Init();


	// main_test();
#endif
#if USINGOLED
	// OLED_Init();
	//oledShowCHStr(0,0,"����Ƕ��ʽ");
#endif
#if USINGESP32
	esp32Init();
#endif
	printf("Mega2560 Test!\r\n");
	ledTimer = Timer_GetTickCount();
	//pwmWrite(PWM_DCM_FOR, 0);
	//pwmWrite(PWM_DCM_BAK, 0);
	//gpioWrite(PA, 3, 1);
	//gpioWrite(PF, 1, 0);
	//gpioWrite(144, 1, 1);//
    ll_lcd();
    keyValue == No_key;
    while(keyValue == No_key)
    {   
        board_process();
        keyValue = keyBoard_read();
        // printf("keyValue = ")
    }
    smdbs = 1;
    uart0_init();
    OLED_Init();
    UART1_send(mla_volume,5);
    UART1_send(js,6);
	Dino_Game();


	
	return 0;
}


