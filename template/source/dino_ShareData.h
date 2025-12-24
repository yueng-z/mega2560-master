#define Esc                     (!(0x01&keyValue) && 0x40&keyValue)
#define up                      (!(0x01&keyValue) && 0x10&keyValue)
#define left                    (!(0x04&keyValue) && 0x10&keyValue)
#define right                   (!(0x08&keyValue) && 0x10&keyValue)
#define normalcy                1
#define hentai                  2
#define begin_pos_on_map        63
#define screen_left             (pos_on_map - begin_pos_on_map)
#define screen_right            (pos_on_map + 64)


#define coin_left    	         x
#define coin_right    	         x+7
#define coin_bottom  	         goldcoin[num][1]
#define coin_top	  	         goldcoin[num][1] + 7
#define  edge_dinox_left         dino_x
#define  edge_dinox_right        dino_x+15
#define  edge_dinoy_head         dino_y+15
#define  edge_dinoy_foot         dino_y


//speed
extern signed char          dino_xspeed;
extern signed char          dino_yspeed;
extern char                 direction;
extern short        		x_sta;
extern short        		y_sta;


//position
extern signed int           pos_on_map;
extern short                dino_x;
extern short*               P_dino_x; 
extern short                dino_y;
extern short                last_dino_y;
extern unsigned int         dino_pos_state;
extern signed char          dino_deep;//小恐龙头部所在行


//state
extern char                 foothold;
extern char				    *p_foothold;
extern char                 jumping;
extern signed int           jump_start_y;
extern char 		        control;
extern char                 Rise;
extern char				    castle_sign;
extern char				    cover;
extern char				    dino_die;
extern char				    finish;
extern signed char          choose;

//data
extern short 				temporary_num;//公用计时器，用完要归零
extern char 				temporary_num1;
extern char 				temporary_num2;
extern char				    life;
extern char 				Array_row;
extern char 				Array_column;
extern char                 level;
extern unsigned int         score;
// extern unsigned int         last_score;
extern unsigned int         Draw_num;
extern unsigned int         max_score;
extern unsigned int         SEG7_number;
extern unsigned char        SEG7_output[4];

extern char                 mla_volume[];
extern char                 mla_bgm[];
extern char                 mla_1up[];
extern char                 mla_jump[];
extern char                 mla_coin[];
extern char                 mla_coin[];
extern char                 mla_bump[];
extern char                 mla_die[];
extern char                 js[];
extern char                 mla_finish[];

extern char                 cxk[];




