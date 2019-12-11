/*
 * isc.h
 *
 *  Created on: Oct 14, 2019
 *      Author: Nick
 */


#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>
/* * P1.6 --> SDA * P1.7 -> SCL * EUSCI_B0 */
void config_i2c(void);
void write_register(uint8_t address, uint8_t reg, uint8_t value);
uint8_t read_register(uint8_t address, uint8_t reg);
void send_start(void);
void send_stop(void);
void set_as_transmitter(void);
void set_as_receiver(void);
/* change the slave address being communicated with */
void set_i2c_address(uint8_t addr);
void set_i2c_byte_counter(uint8_t n);
#endif /* I2C_H_ */
