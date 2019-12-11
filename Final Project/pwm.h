/*
 * pwm.h
 *
 *  Created on: Oct 14, 2019
 *      Author: Nick
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

/* DEFINES */
#define SYSTEM_CLOCK 3000000 // [Hz] system_msp432p401r.c
#define PWM_FREQUENCY 100000 // [Hz] PWM frequency desired
#define CALC_PERIOD(X) (SYSTEM_CLOCK / X) //calc # of ticks in period

void config_pwm_timer(void);
/* Configure TIMER_A0 to produce PWM waveform
 * - TODO: reset R (timer counter) register
 * - TODO: select SMCLK (3MHz) in the CTL register
 * - TODO: select reset/set output mode for T0.1 timer using CCTL[1]
 */

void start_pwm(uint8_t duty_cycle);
/* - TODO: Start PWM signal on Pin XX at duty_cycle 100kHz,
 * Note: the DRV2605L PWM input frequency is XXXX
 * - TODO: calculate and set the amount of ticks needed in CCR
 * - TODO: enable/start timer (UP mode)
 * - TODO: Counting and then reset
 * @param uint8_t duty_cycle: 0-100, percentage of time ON */

void stop_pwm(void);
/* Stop Mode: clear all Mode Control bits, MC, in TAxCTL register */

void config_pwm_gpio(void);
/* Config P2.4 to output TA0.1 waveform */

#endif /* PWM_H_ */
