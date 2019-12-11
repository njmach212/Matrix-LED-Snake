/*
 * UART.h
 *
 *  Created on: Nov 10, 2019
 *      Author: Nick
 */

#ifndef UART_H_
#define UART_H_
void config_UART(void);
void IRQHandler(void);
int receive_string(uint8_t *str,uint16_t size);
int send_string(uint8_t *str,uint16_t length);

#endif /* UART_H_ */
