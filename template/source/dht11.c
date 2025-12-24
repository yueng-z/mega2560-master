
#include "timer.h"
#include "dht11.h"
#include "Delay.h"
#include <stdio.h>


#if USINGDHT11
/*
void delay_ms(int n)
{
	U16 delayTimer = Timer_GetTickCount();
	while(!Timer_PassedDelay(delayTimer, n));
}*/
void DHT11_Rst(void)	   
{  
 	GPIO_IN();	//DQ=1 
	delay_ms(1);
	GPIO_LOW();
	delay_ms(20);    	//拉低至少18ms
	GPIO_IN();	//DQ=1 
	delay_20us    	//主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	GPIO_IN();	 
    while (GPIO_READ()&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_1us
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!GPIO_READ()&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_1us
	};
	if(retry>=100)return 1;	    
	/*
	else retry=0;
    while (GPIO_READ()&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_1us
	};
	if(retry>=100)return 1;	    */
	return 0;	
}

//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(GPIO_READ()&& (retry<100))//等待变为低电平
	{
		retry++;
		delay_1us
	}
	retry=0;
	while(!GPIO_READ()&&(retry<100))//等待变高电平
	{
		retry++;
		delay_1us
	}
	/*
	retry=0;
	while(GPIO_READ()&&(retry<100))//等待变高电平
	{
		retry++;
		delay_1us
	}*/
	delay_40us//等待40us
	if(GPIO_READ())return 1;
	//if(retry>30)return 1;
	else return 0;		   
}

//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_Read_Byte(void)    
{        
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
		PORTB ^= (1<<7);
	}						    
	return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	CLI();  
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		SEI(); // re-enable interrupts		
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{		
			*humi=buf[0];
			*temp=buf[2];
		}
	}	
	else return 1;
	return 0;	    
}

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    		 
u8 DHT11_Init(void)
{	 
	DDRB &= ~(1<<6);
	PORTB |= (1<<6);
	DHT11_Rst();
	return DHT11_Check();
}


#endif
