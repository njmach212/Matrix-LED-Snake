/*
 * matrixLED.h
 *
 *  Created on: Nov 26, 2019
 *      Author: Nick
 */
#include<stdint.h>
#ifndef MATRIXLED_H_
#define MATRIXLED_H_

void start_screen_matrix(uint8_t M[32][32]);
void game_over_matrix(uint8_t M[32][32]);
void read(uint8_t matrix[32][32]);
void config_matrix_led(void);
void test(void);
void reset_matrix(uint8_t matrix[32][32]);

#endif /* MATRIXLED_H_ */
