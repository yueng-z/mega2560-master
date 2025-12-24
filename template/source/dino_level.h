#define __dino_level1  1

extern const short      castle_pos[1];
extern const short      flag_pos[1];
extern const short      original_goldcoin[][3];
extern short            goldcoin[][3];
extern short            coin_num;
extern const short      square[][2];
extern short            square_num;
extern const short   	original_chicken[][6];
extern short   		    chicken[][6];
extern short            chicken_num;
extern int              last_xy[][2];
extern signed char      last_deep[][1];
extern const short   	pit[][2];
extern short            pit_num;

void level_init();