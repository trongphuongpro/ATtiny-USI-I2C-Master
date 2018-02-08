#include <USI-I2C-master.h>

  //////////////////////////////////
 // INITIALIZE USI-I2C AS MASTER //
//////////////////////////////////
void USI_I2C_master_init() {
	USI_DDR |= (1 << USI_SCL) | (1 << USI_SDA);
	USI_PORT |= (1 << USI_SCL) | (1 << USI_SDA);
	
	USICR = (1 << USIWM1) | (1 << USICS1) | (1 << USICLK);	// External, positive edge; software clock strobe (USITC)
	USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC);	// clear Interrupt Flags and Counter.
	USIDR =	0xFF;
}

  /////////////////////////////////////////
 // TRANSFER START CONDITION AND SLA+RW //
/////////////////////////////////////////
uint8_t USI_I2C_master_start(uint8_t address, uint8_t mode) {
	// release and wait for SCL, SDA to HIGH
	releaseSCL();
	while (!(USI_PORT & (1 << USI_SCL)));
	releaseSDA();
	while (!(USI_PORT & (1 << USI_SDA)));
	
				
	// generate START
	pullSDA();
	_delay_us(LOW_PERIOD);
	pullSCL();
	_delay_us(HIGH_PERIOD);
	releaseSDA();
	//********************
	
	// transmit ADDRESS
	pullSCL();	// pull SCL to LOW
	USIDR = (address << 1) + mode;	// mode = 1 - READ, 0 - WRITE, load SLA+W to USIDR
	USI_I2C_master_transfer(USI_DATA);	// transfer SLA+W
	
	USI_DDR &= ~(1 << USI_SDA);	// set SDA as input to read ACK
	
	if (USI_I2C_master_transfer(USI_ACK) & 0x01) 	// read ACK from SLAVE
		return 1;
	
	return 0;
}

  /////////////////////////////
 // TRANSFER STOP CONDITION //
/////////////////////////////
void USI_I2C_master_stop() {
	// release and wait for SCL, SDA to HIGH
	pullSDA();
	releaseSCL();
	while (!(USI_PORT & (1 << USI_SCL)));
	
	// generate STOP
	_delay_us(LOW_PERIOD);
	releaseSDA();
	_delay_us(HIGH_PERIOD);
	//********************
}

  ///////////////////
 // TRANSMIT DATA //
///////////////////
uint8_t USI_I2C_master_transmit(uint8_t data) {
	pullSCL();	// pull SCL to LOW
	USIDR = data;	// load data to USIDR
	USI_I2C_master_transfer(USI_DATA);	// transfer data
	
	USI_DDR &= ~(1 << USI_SDA);	// set SDA as input
	
	if (USI_I2C_master_transfer(USI_ACK) & 0x01) 	// read ACK from SLAVE
		return 1;
	
	return 0;
}

  //////////////////
 // RECEIVE DATA //
//////////////////
uint8_t USI_I2C_master_receive(uint8_t mode) {
	// set SDA as INPUT
	USI_DDR &= ~(1 << USI_SDA);
	// read a byte
	uint8_t data = USI_I2C_master_transfer(USI_DATA);
	if (mode == NOTLAST) 
		USIDR = 0x7F;
	else
		USIDR = 0xFF;
	
	USI_I2C_master_transfer(USI_ACK);
	return data;
}

  ////////////////
 // SHIFT DATA //
////////////////

uint8_t USI_I2C_master_transfer(uint8_t mode) {
	// Respond ACK, NACK
	if (mode == USI_ACK) 
		USISR = 0xFE;
	// transmit Data
	else if (mode == USI_DATA) 
		USISR = 0xF0;

	// Data transmission
	do {
		USICR |= (1 << USITC);  // Positive SCL edge
		while (!(USI_PORT & (1 << USI_SCL)));
		_delay_us(HIGH_PERIOD);
		USICR |= (1 << USITC);	 // Negative SCL edge
		_delay_us(LOW_PERIOD);
	} while (!(USISR & (1 << USIOIF)));
	
	uint8_t data = USIDR;		
	
	// Pre-load data register with "released level" data.
	USIDR = 0xFF;	
	// Release SDA and set as OUTPUT
	releaseSDA();
	USI_DDR |= (1 << USI_SDA);	
	
	return data;
}
