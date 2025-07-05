#include <mega328p.h>
#include <delay.h>
#include "soft_i2c.h"

#define SDA_PORT PORTC
#define SDA_DDR  DDRC
#define SDA_PIN  PINC
#define SDA_BIT  4

#define SCL_PORT PORTC
#define SCL_DDR  DDRC
#define SCL_PIN  PINC
#define SCL_BIT  5

#define SDA_HIGH() (SDA_DDR &= ~(1 << SDA_BIT))
#define SDA_LOW()  (SDA_DDR |= (1 << SDA_BIT))
#define SCL_HIGH() (SCL_DDR &= ~(1 << SCL_BIT))
#define SCL_LOW()  (SCL_DDR |= (1 << SCL_BIT))


void i2c_delay(void) {
    delay_us(15);
}

void i2c_Init(void) {
    SDA_HIGH();
    SCL_HIGH();
}

void i2c_Start(void) {
    SDA_HIGH();
    SCL_HIGH();
    i2c_delay();
    SDA_LOW();
    i2c_delay();
    SCL_LOW();
}

void i2c_Stop(void) {
    SDA_LOW();
    SCL_HIGH();
    i2c_delay();
    SDA_HIGH();
    i2c_delay();
}

void i2c_Write(unsigned char data) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        if (data & 0x80) SDA_HIGH(); else SDA_LOW();
        data <<= 1;
        SCL_HIGH();
        i2c_delay();
        SCL_LOW();
    }

    // ACK bit
    SDA_HIGH();
    SCL_HIGH();
    i2c_delay();
    SCL_LOW();
}

unsigned char i2c_Read(unsigned char ack) {
    unsigned char data = 0; 
    unsigned char i;
    SDA_HIGH(); // release SDA

    for (i = 0; i < 8; i++) {
        data <<= 1;
        SCL_HIGH();
        if (SDA_PIN & (1 << SDA_BIT)) data |= 1;
        i2c_delay();
        SCL_LOW();
    }

    if (ack) SDA_LOW(); else SDA_HIGH();
    SCL_HIGH();
    i2c_delay();
    SCL_LOW();
    SDA_HIGH();

    return data;
}