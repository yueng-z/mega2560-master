/*************************************************************************
* Title: The Key driver Inteface Implementation  File
* 
* File:  key.c 
* Author: lvyi
*
* Discription: This file contains key driver  function

**************************************************************************/

#include "board.h"
#include "key.h"
#include "timer.h"
unsigned char keyValueBoard;

static U16 KeyScan_Timer;

void keyboard_init(void)
{  
	DDRC |= 0x70;
	DDRC &= 0xF0;
	KeyScan_Timer = Timer_GetTickCount();
}



static unsigned char keyboardScan(void)
{	
	static unsigned char key_state = 0, key_value, key_line;
    unsigned char key_return = No_key,i;
	
	switch (key_state)
	{
		case 0:
			key_line = 0b01000000;
			for (i=0; i<3; i++)			// ɨ�����
			{	
				PORTC = 0x7F&(~key_line);			// ������ߵ�ƽ
				PORTC = 0x7F&(~key_line);			// ������2�Σ�����
                key_value = Key_mask & PINC;// ���е�ƽ
				if (key_value == Key_mask)
					key_line >>= 1;			// û�а���������ɨ��
				else
				{
					key_state++;			// �а�����ֹͣɨ��
					break;					// ת����ȷ��״̬
				}
			}
			break;
		case 1:
			if (key_value == (Key_mask & PINC))	// �ٴζ��е�ƽ��
			{
				key_return = 0x7F&(key_line | key_value);// ��״̬0����ͬ��ȷ�ϰ���		
                //Print_Binary(key_return);
				key_state++;				 // ת��ȴ������ͷ�״̬
			}
			else
				key_state--;				 // �����е�ƽ��ͬ����״̬0��������������
			break;						
		case 2:							     // �ȴ������ͷ�״̬
			//PORTC = 0b00001111;			     // ����ȫ������͵�ƽ
			//PORTC = 0b00001111;	             // �ظ���һ��
            //   
			if ( (Key_mask & PINC) == Key_mask)
			{
				key_state=0;				 // ����ȫ��Ϊ�ߵ�ƽ����״̬0
				key_return = No_key;
				break;
			}

			key_value = Key_mask & PINC;
			key_return = 0x7F&(key_line | key_value);
			break;
			//Print_Binary(key_return);

	}
	return key_return;
}



unsigned char keyBoard_read(void)
{
	volatile uint8_t temp = keyValueBoard;
	keyValueBoard = No_key;
	return temp;

}


//--------------------------------------------------
// Scan the keyboard and response the key
//
void keyBoard_process(void)
{
	if(Timer_PassedDelay(KeyScan_Timer, 10)) 
	{		
		KeyScan_Timer = Timer_GetTickCount();
		keyValueBoard = keyboardScan();
	}
}

