#include "Dino_Game.h"
//菜单
#include "dino_menu.h"
//数据
#include "dino_ShareData.h"
#include "board.h"
//恐龙
#include "dino.h"
//地图
#include "dino_level.h"
#include "dino_Pit.h"
#include "dino_Flag.h"
#include "dino_Castle.h"
#include "dino_CACTUS.h"
#include "dino_Square.h"
#include "dino_Goldcoin.h"
#include "dino_Drawmap.h"
#include "dino_Score.h"
//生物
#include "dino_Chicken.h"
//结束
#include "dino_over.h"

//void chicken_init();
void Dino_Game()
{
	char keyValue;
	while (1)
	{
		// while(1)
		// {
		// 	OLED_DrawCover();
		// 	Draw_Tutorial_on_keystrokes();
		// 	choose = level_choose();
		// 	if (choose) break;
		// }
		choose = 0;
		while(1)
		{
			Vital_Display();
			while (1)
			{
				// if (Timer_PassedDelay(ledTimer, 200))
				// {
				// 	ledTimer = Timer_GetTickCount();
				// 	gpioToggle(ledPort, ledPin);
				// }
				board_process();
				#if USINGESP32		
					#if !ESP32_DEBUG
						esp32Process();
					#endif
				#endif
				
				/* 		CLI();
				while(!Queue_IsEmpty(q)&&(UCSR0A & (1 << UDRE0)))
				{
					UDR0 = Queue_Pop(q);
				}	
				SEI();		 */

				// keyValue = keyBoard_read();
				//score ++;
				
				keyValue = keyBoard_read();
				OLED_dino_control(keyValue);
				if (choose) break;
				//printf("life = %d",life);
				OLED_dino_pos();
				OLED_Drawmap();
				if(dino_yspeed != 0 || dino_y % 8 != 0)OLED_DrawDinoJump();
				else OLED_DrawDino();

				//if(shit());
				// {
				//     printf("h");
				// }
				die_judge();
				if(finish)
				{
					break;
				}
                SEG7_calculate_number();
				delay_ms(40);

				//OLED_DrawCactus();
				// if (keyValue != No_key)
				// {
				// 	buzzWorkOnce();
				//     #if USINGDHT11
				//         if(!DHT11_Read_Data(&temp, &humi))		
				//         {
				//             printf("The temp is %d\r\n", temp);	
				//         }
				//     #endif
				// }

			}
			if (choose) break;
			if (finish == 1)
			{
				finish = 0;
				Win_Animation();
			}
			else if (finish == 2)
			{
				finish = 0;
				big_Restart();
			}
		}
	}
	
    
}