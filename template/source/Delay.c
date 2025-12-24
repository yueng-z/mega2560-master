#include "Delay.h"

//单片机主频为16MHZ
void delay_1us(void)
{
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
}


//延时微秒
void delay_us(unsigned int n)
{
    unsigned int i;
    for(i=0;i<n;i++)
        delay_1us();
}


//延时毫秒
void delay_ms(unsigned int n)
{
    unsigned int i;
    for(i=0;i<n;i++)
        delay_us(1000);
}

//延时�?
void delay_s(unsigned int n)
{
    unsigned int i;
    for(i=0;i<n;i++)
        delay_ms(1000);

}
