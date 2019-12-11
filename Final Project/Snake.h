/*
 * Snake.h
 *
 *  Created on: Dec 2, 2019
 *      Author: Nick
 */
#include <stdint.h>
#ifndef SNAKE_H_
#define SNAKE_H_
#define UP 0x03
#define DOWN 0x04
#define RIGHT 0x02
#define LEFT 0x01
void config_snake(uint8_t matrix[32][32]);
int snake_play(uint8_t matrix[32][32], int *Dec);
void ADC14_INIT(void);
void ADC14_GPIO_CONFIG(void);
void getDir(int *Dir);
void put_Dir(int Dir);



#endif /* SNAKE_H_ */
