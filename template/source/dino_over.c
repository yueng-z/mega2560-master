#include "dino_over.h"
#include "dino_ShareData.h"
#include "key.h"
#include "oled.h"

// extern short   original_chicken[][6];
// extern short   chicken_num;
// extern short   chicken[][6];

const unsigned char cai[128] = 
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0xF0,0xF0,0x80,0x80,0x80,0x80,0x80,0x80,0xE0,0xF0,0xFC,0xDC,0xCC,0x40,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x23,0x23,0x33,0x37,0x37,0xD9,0xD9,0x58,0x8E,0xCF,0xED,0x6C,0x3C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x08,0x38,0x18,0x18,0x18,0x89,0xCF,0x6D,0x7C,0x3C,0x1E,0xFF,0xFF,0x1F,0x1B,0x32,0x33,0x23,0x43,0xC3,0x83,0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x08,0x0C,0x0C,0x0E,0x06,0x03,0x01,0x01,0x10,0x10,0x10,0x10,0x30,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x07,0x07,0x07,0x07,0x06,0x06,0x04,0x00,/*"菜",0*/
};
extern const unsigned char  RESTART[];
uint8_t keyValue;


//死亡判断
void die_judge()
{
	if (dino_y + 15 < - 30)
	{
		dino_die = 1;
	}
	// if (dino_die == 1)
	// {
    //     UART1_send(mla_die,6);
	// 	if (life > 0)
	// 	{
	// 		Die_Animation(No_key);
	// 		wait_key();
	// 		oledClear();
	// 		Vital_Display(No_key);
	// 		small_Restart();
	// 	}
	// 	else
	// 	{
	// 		finish = 2;
    //         score = 0;
	// 	}
	// }
	dino_die = 0;
}

//胜利
void Win_Animation()
{
    
    oledClear();
    oledShowString(24,2,"you Finish!");
    oledShowString(38,4,"score:");
    delay_ms(1000);
    oledShowString(38,4,"      ");
    keyValue = No_key;
    while(keyValue == No_key)
    {
        board_process();
        keyValue = keyBoard_read();
        if (keyValue != No_key)
        {
            Draw_num = score;
        }
        
        if (Draw_num < score)
        {
            oledShowNum(40,4,Draw_num,5,16);
            Draw_num += 3;
        }
        else
        {
            oledShowNum(40,4,score,5,16);
            if(score >= max_score)
            {
                oledShowString(40,6,"best!");
                max_score = score;
            } 
        }

    }
    delay_ms(500);
    score = 0;
    big_Restart();
}

//生命显示
void Vital_Display()
{
	char j, byte, dino_dir = 0;

    keyValue = No_key;
    while(keyValue == No_key)
    {
        board_process();
        keyValue = keyBoard_read();
        for(Array_column = 0; Array_column< 2; Array_column++)
        {
            OLED_SetPos(30, 3+Array_column);
            for (Array_row = 0; Array_row < 16; Array_row++)
            {
                j = Array_column*16 + Array_row;
                byte = rightDINO[dino_dir][j];
                oledWriteByte(byte,OLED_DATA);
            }
        }
        dino_dir++;
        dino_dir = dino_dir%2;
        oledShowChar(50,3,'x');
        oledShowNum(64,3,life,1,16);
        if (dino_die == 1)
        {
            oledShowString(80,3,"->");
            oledShowNum(100,3,life - 1,1,16);
            dino_die = 0;
            life--;
        }
        delay_ms(40);
    }
    delay_ms(500);
    oledClear();
}

//死亡
void Die_Animation()
{
	temporary_num1 = 0;
	temporary_num2 = 0;
	for (Array_column = 0; Array_column < 4; Array_column++)
	{
		oledSetPos(47,2+Array_column);
		for (Array_row = 0; Array_row < 32; Array_row++)
		{
			temporary_num1 = 32*Array_column + Array_row;
			temporary_num2 = cai[temporary_num1];
			oledWriteByte(temporary_num2,OLED_DATA);
		}
	}
}

//等待按键
void wait_key()
{
	char keyValue;
	keyValue = No_key;
	delay_ms(500);
	while (keyValue == No_key)
	{
		board_process();
		keyValue = keyBoard_read();
	}

}

// 小重启
void small_Restart()
{
	oledClear();
	dino_x = 0;
	pos_on_map = begin_pos_on_map;
    Rise = 1;
    level_init();
    score = 0;
    // last_score = 0;
}

//大重启
void big_Restart()
{
    unsigned char byte;
    unsigned int j=0;

    if (finish  == 2)
    {
        for (Array_column = 2; Array_column < 5; Array_column++)
        {
            OLED_SetPos(52, Array_column);
            for (Array_row = 0; Array_row < 24; Array_row++)
            {
                byte = RESTART[j++];
                oledWriteByte(byte,OLED_DATA);
            }
        }
    }
    wait_key();
    small_Restart();
    life = 3;
}