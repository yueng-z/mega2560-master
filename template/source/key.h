

extern unsigned char keyValueBoard;


#define No_key 	255
#define K1_1	0b01001110
#define K1_2	0b01001101
#define K1_3	0b01001011
#define K1_4	0b01000111
#define K2_1	0b00101110
#define K2_2	0b00101101
#define K2_3	0b00101011
#define K2_4	0b00100111
#define K3_1	0b00011110
#define K3_2	0b00011101
#define K3_3	0b00011011
#define K3_4	0b00010111

#define Key_mask	0b00001111

extern unsigned char keyBoard_read(void);
extern void keyBoard_process(void);
extern void keyboard_init(void);