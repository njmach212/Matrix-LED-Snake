/*
 * pwm.c
 *
 *  Created on: Oct 14, 2019
 *      Author: Nick
 */

# include "msp.h"
#include "pwm.h"
#include <stdio.h>



void config_pwm_timer(void) {
    TIMER_A2->CTL |= TIMER_A_CTL_CLR; // Reset R (timer counter) register
    TIMER_A2->CTL |= TIMER_A_CTL_SSEL__SMCLK; // Select SMCLK (3MHz) in the CTL register
    TIMER_A2->CCTL[2] |= TIMER_A_CCTLN_OUTMOD_7; // Select reset/set output mode for T0.1 timer using CCTL[1]
}

void start_pwm(uint8_t duty_cycle) {
    uint8_t T = CALC_PERIOD(PWM_FREQUENCY);
    TIMER_A2->CCR[0] |= T; // Value for TA0CCR0
    TIMER_A2->CCR[2] |= (duty_cycle*T) / 100; // Value for TA0CCR1
    TIMER_A2->CTL |= TIMER_A_CTL_MC_1; // Set to up mode

}

void stop_pwm(void) {
    TIMER_A2->CTL &= ~TIMER_A_CTL_CLR; // Clear timerA
}

void config_pwm_gpio(void) {
    P5->DIR |= BIT7;
    P5->SEL0 |= BIT7; // selecting secondary module on p2 pin4
    P5->SEL1 &= ~BIT7; // selecting secondary module on p2 pin4
}

