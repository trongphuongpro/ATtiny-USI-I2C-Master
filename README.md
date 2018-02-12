# ATtiny-USI-I2C-Master


## Usage & API
```c #include <USI-I2C-master.h> 
```
- void **USI_I2C_master_init()** - Initialize a I2C Master device.
- uint8_t **USI_I2C_master_start(uint8_t address, uint8_t mode)** - Start a transmission with modes: USI_READ, USI_WRITE.
- void **USI_I2C_master_stop()** - Terminate the transmission.
- uint8_t **USI_I2C_master_transmit(uint8_t data)** - Transmit a byte to Slave
- uint8_t **USI_I2C_master_receive(uint8_t mode)** - Receive a byte from Slave, with modes: NOTLAST, LAST.
- uint8_t **USI_I2C_master_transfer(uint8_t data)** - Core function for shifting data in/out register USIDR.
