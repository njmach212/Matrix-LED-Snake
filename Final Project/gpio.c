/*
 * gpio.c
 *
 *  Created on: Oct 14, 2019
 *      Author: Nick
 */
#include "gpio.h"
#include "pwm.h"
#include "msp.h"
#include "drv2605l.h"
#include "i2c.h"


// set i2c pins on msp to connect to drv
// connect pwm pin on msp to drv
void config_drv_gpio(void){
    /*P1->DIR |= BIT6; //sets SDA to output
    P1->DIR |= BIT7; //sets SCL to output
    P1->OUT |= BIT6;
    P1->OUT |= BIT7;*/
    P3->DIR |= BIT0;  //selecting p2 pin 6 for the output to enable the EN pin on drv
    P3->OUT &= ~BIT0; // set p2 pin 6 to low for to enable the EN pin
}
void toggle_routine(void){
    P3->OUT ^= BIT0; //toggle the EN pin to put drv in and out of standby
}

