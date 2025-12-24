#ifndef __DHT11_H
#define __DHT11_H 
#include "board.h"
#include <iom2560v.h>
#include <macros.h>


#define delay_1us \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();

#define delay_10us  delay_1us\
                    delay_1us\
                    delay_1us\
                    delay_1us\
                    delay_1us\
                    delay_1us\
                    delay_1us\
                    delay_1us\
                    delay_1us\
                    delay_1us

#define delay_20us  delay_10us\
                    delay_10us
#define delay_30us  delay_10us\
                    delay_20us
#define delay_40us  delay_20us\
                    delay_20us

#define GPIO_IN() DDRB&=~(1<<6); PORTB|=(1<<6)
#define GPIO_LOW() DDRB|=(1<<6); PORTB&=~(1<<6)
#define GPIO_READ() (PINB&(1<<6))

void DHT11_Rst(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi);
uint8_t DHT11_Init(void);

//void delay_ms(int n);

#endif





/*
static void delay_us(unsigned int time){
	//cli();	
	switch (time)
	{
	case 1:
		delay_1us
		break;
	case 10:
		delay_10us
		break;	
	case 20:
		delay_20us
		break;
	case 30:
		delay_30us
		break;
	case 40: 
		delay_40us
		break;
	default:
		break;
	}
	//sei();
}  

*/









