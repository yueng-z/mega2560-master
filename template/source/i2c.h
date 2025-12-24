// ***************     I2C driver V 1.0    ***************
// ***           Written By P. Fletcher-Jones          ***
// ***                Written on 06/6/01			   ***
// ***                 Last MOD 15/11/01			   ***
// ***       Compiled using ImageCraft C Comiler       ***
// *******************************************************

// ****************************** //
// *** I2C Hardware Interface *** //
// ****************************** //

#define SDA_PIN 1
#define SDA_DIR DDRD
#define SDA_PORT PORTD
#define SDA_PINREG PIND

#define SCL_PIN 0
#define SCL_DIR DDRD
#define SCL_PORT PORTD

#define EEPROM_BUS_ADDRESS 0xa0

#define SDA_Input (SDA_PINREG & BIT(SDA_PIN))

#define SCL_OutputLow cbi(SCL_PORT, SCL_PIN)
#define SDA_OutputLow cbi(SDA_PORT, SDA_PIN)

#define SCL_Hight cbi(SCL_DIR, SCL_PIN)
#define SCL_Low sbi(SCL_DIR, SCL_PIN)
#define SDA_Hight cbi(SDA_DIR, SDA_PIN)
#define SDA_Low sbi(SDA_DIR, SDA_PIN)
#define I2C_Delay \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();        \
    NOP();

void eeprom_write(unsigned int address, unsigned char data);
unsigned char eeprom_read(unsigned int address);

void I2C_init(void);
unsigned char I2C_start(void);
void I2C_stop(void);
unsigned char I2C_read(unsigned char ack);
unsigned char I2C_write(unsigned char c);
