#include "dino_Score.h"
#include "oled.h"
#include "dino_ShareData.h"


void OLED_ShowScore()
{
    // static char        length = 0;
    // int                cal_num;
    // static int         get_score = 0;
    // static short       timer = 0;
    // get_score = last_score - score;
    // if (get_score)
    // {
    //     cal_num = get_score;
    //     length = 0;
    //     while(cal_num)
    //     {
    //         cal_num /= 10;
    //         length++;
    //     }
    //     last_score = score;
    //     timer = 50;
    // }
    // if(timer)
    // {

    //     oledShowString(dino_x - 8, dino_y + 16, "+");
    //     oledShowNum(dino_x, dino_y + 16, get_score, length, 8);
    //     timer--;
    // }

    // oledShowString(0,0,"dino:");
	// oledShowNum(0,2,score,5,16);
}

void SEG7_calculate_number()
{
    char    length = 0;
    int     cal_num;
    if (SEG7_number < score)
    {
        SEG7_number+=43;
    }
    else
    {
        SEG7_number = score;
    }
    // printf("SEG7_number = %d   ",SEG7_number);
    cal_num = SEG7_number;
    while(cal_num)
    {
        cal_num /= 10;
        length++;
    }
    cal_num = 1;
    for(Array_column = 0; Array_column < length; Array_column++)
    {
        SEG7_output[3-Array_column] = (SEG7_number%(cal_num*10))/cal_num;
        //printf("SEG7_output[%d] = %d ",3-Array_column,SEG7_output[3-Array_column]);
        cal_num *= 10;
    }
    //printf(" | ");
}

// void oledShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
// {
// 	u8 t, temp;
// 	u8 enshow = 0;
// 	for (t = 0; t < len; t++)
// 	{
// 		temp = (num / oled_pow(10, len - t - 1)) % 10;
// 		if (enshow == 0 && t < (len - 1))
// 		{
// 			if (temp == 0)
// 			{
// 				oledShowChar(x + (size2 / 2) * t, y, ' ');
// 				continue;
// 			}
// 			else
// 				enshow = 1;
// 		}
// 		oledShowChar(x + (size2 / 2) * t, y, temp + '0');
// 	}
// }