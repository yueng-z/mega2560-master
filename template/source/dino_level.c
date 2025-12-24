#include "dino_level.h"
#include <string.h>

// short goldcoin[][3];
// short coin_num = sizeof(goldcoin)/6;
// short coin_num;
// const short   square[][2];
// short square_num = sizeof(square)/4;
// short square_num;

const short     flag_pos[1]= {1090};
const short     castle_pos[1]={1140};

const short   		pit[][2] = {
    200,20,
    237,20,
    500,208,
};
short pit_num = sizeof(pit)/4;


const short     original_goldcoin[][3] = {//x,y,碰撞，上一次碰撞
    5,24,1,
    21,24,1,
    37,24,1,
    69,8,1,
    117,24,1,
    172,8,1,
    222,8,1,
    260,8,1,
    455,8,1,
    463,8,1,
    471,8,1,
    517,24,1,
    573,24,1,
    629,24,1,
    685,24,1,
    801,8,1,
    817,8,1,
    897,8,1,
    913,8,1,
    
};
short   	    goldcoin[][3] = {//x,y,碰撞，上一次碰撞
    5,24,1,
    21,24,1,
    37,24,1,
    69,8,1,
    117,24,1,
    172,8,1,
    222,8,1,
    260,8,1,
    455,8,1,
    463,8,1,
    471,8,1,
    517,24,1,
    573,24,1,
    629,24,1,
    685,24,1,
    801,8,1,
    817,8,1,
    897,8,1,
    913,8,1,
};
short coin_num = sizeof(original_goldcoin)/6;


const short   square[][2] = 
{
    0,5,
    16,5,
    32,5,
    64,7,
    112,7,
	112,6,  
    112,5,
    300,7,
    367,7,
    433,7,
    540,7,
    596,7,
    652,7,
    750,7,

    780,3,
    796,3,
    812,3,
    828,3,
    // 796,1,
    // 812,1,

    860,3,
    876,3,
    892,3,
    908,3,
    // 892,1,
    // 908,1,

    940,7,
    

    970,7,
    986,6,
    1002,5,
    1018,4,
    1034,3,

};
short square_num = sizeof(square)/4;


const short   		original_chicken[][6] = {//70x,0y,2speed,0碰撞，0上一次碰撞,0start_y，last上一次
    320, 0, 2, 0, 0, 0,
    390, 0, 2, 0, 0, 0,
    800, 0, 2, 0, 0, 0,
    900, 0, 2, 0, 0, 0,
    // 100,0,2,0,0,0,
	// 160,0,2,0,0,0,
};
short   		chicken[][6] = {//70x,0y,2speed,0碰撞，0上一次碰撞,0start_y，last上一次
    320, 0, 2, 0, 0, 0,
    390, 0, 2, 0, 0, 0,
    800, 0, 2, 0, 0, 0,
    900, 0, 2, 0, 0, 0,
    // 100,0,2,0,0,0,
	// 160,0,2,0,0,0,
};
short chicken_num = sizeof(original_chicken)/12;

int             last_xy[][2] = {
    -1,-1,
    -1,-1,
    -1,-1,
    -1,-1,
    -1,-1,
    -1,-1,
    -1,-1,
    -1,-1,
    -1,-1,
};
signed char     last_deep[][1] = {
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};


void level_init()
{
    int x,y;
    //printf("original_chicken = %d",original_chicken);
    for (y = 0; y < chicken_num; y++)
    {
        for (x = 0; x < 6; x++)
        {
            printf("chicken%d = %d == original = %d",x,chicken[y][x],original_chicken[y][x]);
            chicken[y][x] = original_chicken[y][x];
            printf("modif_chicken = %d," ,chicken[y][x]);
        }
    }
    // printf("original_goldcoin = %d",original_goldcoin);
    for (y = 0; y < coin_num; y++)
    {
        for (x = 0; x < 3; x++)
        {
            goldcoin[y][x] = original_goldcoin[y][x];
        }
    }
}



//level1
// short   	level1_goldcoin[][3] = {//x,y,碰撞，上一次碰撞
//     50,8,1,
//     90,8,1,
//     100,8,1,
//     150,8,1,
//     190,8,1,
//     200,8,1,
// };

// short       level1_square[][2] = {
//     // 47,5,
//     47,7,
//     125,7,
//     300,7,
// 	317,7,
//     450,7,
//     500,5,
//     30,5,
//     200,7,
// 	217,7,
// };






// void level_init(char level)
// {
//     int x,y;
//     if (level == 1)
//     {
//         //memcpy(goldcoin,level1_goldcoin,sizeof(level1_goldcoin));
//         coin_num = sizeof(level1_goldcoin)/6;
//         printf("coin_num = %d ",coin_num);

//         for (y = 0; y < coin_num; y++)
//         {
//             for (x = 0; x < 3; x++)
//             {
//                 printf("goldcoin[%d][%d] = %d   ",y,x,goldcoin[y][x]);
//             }
            
//         }
        
//         //memcpy(square,level1_square,sizeof(level1_square));
//         square_num = sizeof(level1_square)/4;
//         printf("square_num = %d ",square_num);

//         for (y = 0; y < square_num; y++)
//         {
//             for (x = 0; x < 3; x++)
//             {
//                 printf("square[%d][%d] = %d   ",y,x,square[y][x]);
//             }
            
//         }
//     }
// }



