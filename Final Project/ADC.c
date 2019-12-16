/*
 * ADC.c
 *
 *  Created on: Dec 5, 2019
 *      Author: Nick
 */
#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "ADC.h"

// Configure P5.0 and P5.2 for ADC input
void ADC14_GPIO_CONFIG(void) {

    //P5.0 and P5.2 -> ADC function

    P5->SEL1 |= (BIT0 | BIT2);

    P5->SEL0 |= (BIT0 | BIT2);

}

// Initialize ADC14 functionality
void ADC14_INIT(void) {

    //Enable Interrupts

    __enable_irq();

    NVIC_SetPriority(ADC14_IRQn, 2);

    NVIC_EnableIRQ(ADC14_IRQn);

    //Configure ADC14

    ADC14->CTL0 = (ADC14_CTL0_SHT1__192 | ADC14_CTL0_SHP | ADC14_CTL0_ON); // Turns ADC14 on, Sample and Hold Pulse mode, and 192 ADCCLK cycles per sample

    ADC14->CTL1 = ADC14_CTL1_RES__12BIT; // 12 bit conversions

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_3; // ADC input channel 3 for P5.0

    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_5; // ADC input channel 3 for P5.2

    ADC14->MCTL[1] |= ADC14_MCTLN_EOS; // set memory control register 1 to end of conversion sequence

    ADC14->IER0 |= ADC14_IER0_IE1; // enable interrupt 1

    ADC14->IER0 |= ADC14_IER0_IE0; // enable interrupt 0

    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_3; // sample and conversion sequence repeat mode

    //Start Conversion

    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC; // begin one cycle of conversions

}


// ISR for Xout on joystick (P5.0)
void ADC14_ISR_Xout(void) {

    //Clear Interrupt Flag

    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0; // clear flag

    int i;

    if((ADC14->MEM[0]) >= 0xFA0) { // if conversion is greater than 4000...
    
        joy_dir = 4

    }

    else if((ADC14->MEM[0]) <= 0x64) { // if conversion less than 100

        joy_dir = 3;

    }

}


// ISR handler for Yout on joystick (P5.2)
void ADC14_ISR_Yout(void) {

    //Clear Interrupt Flag

    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG1;

    int i;

    if((ADC14->MEM[1]) >= 0xFA0) { // If conversion greater than 4000

        joy_dir = 1

    }

    else if((ADC14->MEM[1]) <= 0x64) { // if conversion less than 100

        joy_dir = 2;

    }

}


// Handles IRQ based on priority
void ADC14_IRQHandler(void) {

    //Handle IRQs in ADC14IV

    switch(ADC14->IV) {

    case 0x0C :

        ADC14_ISR_Xout();

    case 0x0E :

        ADC14_ISR_Yout();

    }

}
