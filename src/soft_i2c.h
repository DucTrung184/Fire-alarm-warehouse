#ifndef _SOFT_I2C_H_
#define _SOFT _I2C_H_

void i2c_Init(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_Write(unsigned char data);
unsigned char i2c_Read(unsigned char ack);

#endif
