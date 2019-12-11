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
/*
void ADC14_GPIO_CONFIG(void) {

    //P5.0 and P5.2 -> ADC function

    P5->SEL1 |= (BIT0 | BIT2);

    P5->SEL0 |= (BIT0 | BIT2);

}



void ADC14_INIT(void) {

    //Enable Interrupts

    __enable_irq();

    NVIC_SetPriority(ADC14_IRQn, 2);

    NVIC_EnableIRQ(ADC14_IRQn);

    //Configure ADC14

    ADC14->CTL0 = (ADC14_CTL0_SHT1__192 | ADC14_CTL0_SHP | ADC14_CTL0_ON);

    ADC14->CTL1 = ADC14_CTL1_RES__12BIT;

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_3;

    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_5;

    ADC14->MCTL[1] |= ADC14_MCTLN_EOS;

    ADC14->IER0 |= ADC14_IER0_IE1;

    ADC14->IER0 |= ADC14_IER0_IE0;

    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_3;

    //Start Conversion

    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;

}



void ADC14_ISR_Xout(void) {

    //Clear Interrupt Flag

    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0;

    //Print Joystick Direction

    int i;

    if((ADC14->MEM[0]) >= 0xFA0) {

        printf("Right\n");

        NVIC_DisableIRQ(ADC14_IRQn);

        //for(i = 0; i < 100000; i++);

        NVIC_EnableIRQ(ADC14_IRQn);

    }

    else if((ADC14->MEM[0]) <= 0x64) {

        printf("Left\n");

        NVIC_DisableIRQ(ADC14_IRQn);

        //for(i = 0; i < 100000; i++);

        NVIC_EnableIRQ(ADC14_IRQn);

    }

    //Start Another Conversion

    //ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;

}



void ADC14_ISR_Yout(void) {

    //Clear Interrupt Flag

    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG1;

    //Print Joystick Direction

    int i;

    if((ADC14->MEM[1]) >= 0xFA0) {

        printf("Up\n");

        NVIC_DisableIRQ(ADC14_IRQn);

        //for(i = 0; i < 100000; i++);

        NVIC_EnableIRQ(ADC14_IRQn);

    }

    else if((ADC14->MEM[1]) <= 0x64) {

        printf("Down\n");

        NVIC_DisableIRQ(ADC14_IRQn);

        //for(i = 0; i < 100000; i++);

        NVIC_EnableIRQ(ADC14_IRQn);

    }

    //Start Another Conversion

    //ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;

}



void ADC14_IRQHandler(void) {

    //Handle IRQs in ADC14IV

    switch(ADC14->IV) {

    case 0x0C :

        ADC14_ISR_Xout();

    case 0x0E :

        ADC14_ISR_Yout();

    }

}*/
