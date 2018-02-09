#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#ifndef sbi
	#define sbi(port, bit)	(port) |= (1 << bit)
#endif
#ifndef cbi
	#define cbi(port, bit)	(port) &= ~(1 << bit)
#endif

#if defined(__AVR_ATtiny2313__)
	#define USI_DDR		DDRB
	#define USI_PORT	PORTB
	#define USI_PIN		PINB
	#define USI_SDA		PB5
	#define USI_SCL		PB7
#endif

#if defined(__AVR_ATtiny85__)
	#define USI_DDR		DDRB
	#define USI_PORT	PORTB
	#define USI_PIN		PINB
	#define USI_SDA		PB0
	#define USI_SCL		PB2
#endif

#define releaseSCL()	USI_PORT |= (1 << USI_SCL)
#define releaseSDA()	USI_PORT |= (1 << USI_SDA)
#define pullSCL()	USI_PORT &= ~(1 << USI_SCL)
#define pullSDA()	USI_PORT &= ~(1 << USI_SDA)

#define USI_ACK		0
#define USI_DATA	1

#define USI_READ	1
#define USI_WRITE	0
#define NOTLAST		0
#define LAST		1

#define LOW_PERIOD	4
#define HIGH_PERIOD	5


void USI_I2C_master_init();
uint8_t USI_I2C_master_start(uint8_t, uint8_t);
void USI_I2C_master_stop();
uint8_t USI_I2C_master_transmit(uint8_t);
uint8_t USI_I2C_master_receive(uint8_t);
uint8_t USI_I2C_master_transfer(uint8_t);
