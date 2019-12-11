/**
 * main.c
 */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "UART.h"
#include "matrixLED.h"
#include "Snake.h"
#include <time.h>
#include <stdlib.h>
#include "gpio.h"
#include "drv2605l.h"
#include "pwm.h"
#include "i2c.h"
//#include "ADC.h"

void error(void)
{
    volatile uint32_t i;
    P1->DIR |= BIT0;
    while(1) {
        P1->OUT ^= BIT0;
        for(i = 20000; i > 0; i--); // Blink LED forever
    }
}
void (*fp)(void) = &toggle_routine;
void main(void)
{
    // Transition to VCORE Level 1: AM0_LDO -> AM1_LDO
    uint32_t currentPowerState;
    currentPowerState = PCM->CTL0 & PCM_CTL0_CPM_MASK;
    if(currentPowerState != PCM_CTL0_CPM_0) error(); // error if current power state is not AM0_LDO
    while((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    if(PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG) error(); // error if transition was not successful
    if((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM_1) error(); // error if device is not in AM1_LDO mode
    // Configure Flash wait-state to 1 for both banks 0 & 1
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;
    // Configure HFXT to use 48MHz crystal, source to MCLK
    PJ->SEL0 |= BIT2 | BIT3; // Configure PJ.2/3 for HFXT function
    PJ->SEL1 &= ~(BIT2 | BIT3);
    CS->KEY = CS_KEY_VAL; // unlock CS module for register access
    CS->CTL2 |= CS_CTL2_HFXT_EN | CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE;
    while(CS->IFG & CS_IFG_HFXTIFG)
    {
        CS->CLRIFG |= CS_CLRIFG_CLR_HFXTIFG;
    }
    // Select MCLK = HFXT
    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_DIVM_MASK | CS_CTL1_SELM_MASK) | CS_CTL1_SELM__HFXTCLK;//CS_CTL1_SELS_MASK
    CS->KEY = 0; // Lock CS module from unintended accesses
    // Output MCLK to port pin to demonstrate 48MHz operation
    //P4->DIR |= BIT3;
    // P4->SEL0 |= BIT3;
    //P4->SEL1 &= ~BIT3;


    /*PJ->SEL0 |= (BIT2 |BIT3);
    PJ->SEL1 &= ~(BIT2 |BIT3);
    CS->KEY = 0x695A;//unlocks access to change system and master clock
    CS->CTL2 |= CS_CTL2_HFXTDRIVE;
    CS->CTL2 |= CS_CTL2_HFXT_EN;
    CS->CTL1 &= ~(CS_CTL1_SELS_MASK | CS_CTL1_SELM_MASK);//sets system and master clock bits to zero were originally 3
    CS->CTL2 |= CS_CTL2_HFXTFREQ_6;//sets frequency of HFXT clock to between 40 and 48 MHz
    CS->CTL1 |= CS_CTL1_SELS_5 | CS_CTL1_SELM_5;//sets system and master clock to select HFXT clock
    CS->KEY = 0xA596;*/


    /*__enable_irq();//enables interrupts and interrupt handlers
    config_UART();//configures UARt

    int i, j, u = 0;
    uint8_t length = 1;
    char k[] = "123456789";
    char counter[] = "Counter:";
    char clear[] = "|-";
    for(j = 0; j<1000000000; j++)
    {
        for(i = 0; i<1000000; i++);
        send_string((uint8_t *) counter,strlen(counter));//sends string over UARt by filling TXBuf with the sent string
        for (i=0; i<1000000; i++);//Delay
        send_string((uint8_t *)k[1] ,length);//sends string over UARt by filling TXBuf with the sent string
        for (i=0; i<1000000; i++);//Delay
        send_string((uint8_t *) clear,strlen(clear));//sends string over UARt by filling TXBuf with the sent string
        for (i=0; i<1000000; i++);//Delay
        u++;
        if(u>10)length = 2;
    }*/
    /*char test0[] = "|\xD9";
    char test1[] = "|-";
    char test2[] = "Nick, it works  ";
    char test3[] = "Go Buffs        ";
    for (i=0; i<1000; i++);//Delay
    send_string((uint8_t *) test0,strlen(test0));//sends string over UARt by filling TXBuf with the sent string
    for (i=0; i<1000; i++);//Delay
    send_string((uint8_t *) test1,strlen(test1));//sends string over UARt by filling TXBuf with the sent string
    for (i=0; i<1000; i++);//Delay
    send_string((uint8_t *) test2,strlen(test2));//sends string over UARt by filling TXBuf with the sent string
    for (i=0; i<1000; i++);//Delay
    send_string((uint8_t *) test3,strlen(test3));//sends string over UARt by filling TXBuf with the sent string
    for (i=0; i<1000; i++);//Delay*/

    srand(time(NULL));
    int i, end = 1, sel = 0, DIR = 0, j, dec = 0, d = 0;
    uint8_t matrix[32][32] = {};
    uint8_t dc = 90;
    config_i2c();
    config_pwm_gpio();
    config_pwm_timer();
    config_drv2605L(*fp);
    config_drv_gpio();
    __enable_irq();//enables interrupts and interrupt handlers
    config_UART();//configures UARt
    config_matrix_led();
    start_screen_matrix(matrix);
    read(matrix);
    ADC14_GPIO_CONFIG();
    ADC14_INIT();
    start_pwm(dc);
    while(1)
    {
        ADC14->CTL0 |= ADC14_CTL0_SC;
        getDir(&DIR);
        read(matrix);
        if(DIR != 0)
        {
            reset_matrix(matrix);
            config_snake(matrix);
            for(i = 0; i<50; i++);
            put_Dir(0);
            end = 0;
        }
        while(end == 0)
        {
            d = 0;
            for(i = 0; i<30; i++)
            {
                read(matrix);
                ADC14->CTL0 |= ADC14_CTL0_SC;
            }
            P3->OUT &= ~BIT0;
            end = snake_play(matrix, &dec);
            /*if(dec == 1)
            {
                read(matrix);
                for(j = 0; j<2000000; j++);
                toggle_routine();
            }*/
        }
        if(end == 2)
        {
            read(matrix);
            if(d == 0)
            {
                read(matrix);
                toggle_routine();
                for(j = 0; j<2000000; j++);
                toggle_routine();
                d = 1;
            }
            reset_matrix(matrix);
            game_over_matrix(matrix);
            for(i = 0; i<500; i++)
            {
                read(matrix);
            }
            end = 1;
        }
            reset_matrix(matrix);
            read(matrix);
            start_screen_matrix(matrix);
            read(matrix);
            ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
            getDir(&DIR);
            if(DIR != 0) sel = 1;
            if(sel == 1) end = 0;
    }
}
