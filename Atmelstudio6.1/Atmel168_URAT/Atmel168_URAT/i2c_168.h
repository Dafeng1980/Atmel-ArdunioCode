/*
 * i2c_168.h
 *
 * Created: 2015/6/18 9:20:20
 *  Author: PE_NPI
 */ 


#ifndef I2C_168_H_
#define I2C_168_H_

#include <avr/io.h>
#include <stdio.h>
#include <compat/twi.h>
#define MAX_TRIES 50
#define I2C_START 0
#define I2C_DATA 1
#define I2C_DATA_ACK 2
#define I2C_STOP 3
#define ACK 1
#define NACK 0

extern unsigned char i2c_transmit(unsigned char type);
extern char i2c_start(unsigned int dev_id, unsigned int dev_addr, unsigned char rw_type);
extern void i2c_stop(void);
extern char i2c_write(char data);
extern char i2c_read(char *data,char ack_type);
extern void uart_init (unsigned int ubrr);
extern int uart_putch(char ch,FILE *stream);
extern int uart_getch(FILE *stream);
extern void ansi_cl(void);
extern void ansi_me(void);
void ansi_cm(unsigned char row,unsigned char col);

//FILE uart_str = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);
#endif /* I2C_168_H_ */