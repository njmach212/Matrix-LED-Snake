/*
 * gpio.h
 *
 *  Created on: Oct 14, 2019
 *      Author: Nick
 */

#ifndef GPIO_H_
#define GPIO_H_

void config_drv_gpio(void);
//sets up p2.4 to be an output for the pwm signal to be sent
// sets up p2.6 to be an ouput and to enable the EN pin on the DRV

void toggle_routine(void);
//toggles p2.6 to turn on and off the EN pin putting the drv in and out of standby mode



#endif
