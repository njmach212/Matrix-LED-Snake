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

void error(void)//error for clock chnage 
{
    volatile uint32_t i;
    P1->DIR |= BIT0;
    while(1) {
        P1->OUT ^= BIT0;
        for(i = 20000; i > 0; i--); // Blink LED forever
    }
}
void (*fp)(void) = &toggle_routine;//toggle routine for LRA on and off
void main(void)
{
    // CONFIGURE MCLK TO 48MHZ (from: https://e2echina.ti.com/question_answer/microcontrollers/other_mcu/f/23/p/139193/391432)
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

    srand(time(NULL)); // initialize random number generator for apples
    int i, end = 1, sel = 0, DIR = 0, j, dec = 0, d = 0;
    uint8_t matrix[32][32] = {}; // initialize zero matrix
    uint8_t dc = 90; // pwm frequency for LRA
    
    // config peripherals
    config_i2c();
    config_pwm_gpio();
    config_pwm_timer();
    config_drv2605L(*fp);
    config_drv_gpio();
    
    __enable_irq();//enables interrupts and interrupt handlers
    config_UART();//configures UARt
    config_matrix_led(); // configure pins for matrix led
    start_screen_matrix(matrix); // create the start screen for snake
    read(matrix); // read matrix values and update matrix LED pins accordingly
    ADC14_GPIO_CONFIG(); // config adc
    ADC14_INIT(); // initialize adc
    start_pwm(dc); // initialize pwm signal on MSP432
    while(1)
    {
        ADC14->CTL0 |= ADC14_CTL0_SC; // begin adc conversion
        getDir(&DIR); // get joystick direction based on adc conversion
        read(matrix); // read matrix values and update display accordingly
        if(DIR != 0)
        {
            reset_matrix(matrix);//resets matrix to all 0's turning all led's off
            config_snake(matrix);//sets up the snake game at starting position for snake and apple
            for(i = 0; i<50; i++);//delay
            put_Dir(0);//put direction to 0
            end = 0;//set end to zero so that it will go into next whiel loop
        }
        while(end == 0)//whiel end is 0
        {
            d = 0;//d = 0
            for(i = 0; i<30; i++)//sets how fast the snake game play is based on how high for loop goes
            {
                read(matrix);//read matrix and update led accordingly
                ADC14->CTL0 |= ADC14_CTL0_SC;//start conversions
            }
            P3->OUT &= ~BIT0;//turn enable pin for LRA off
            end = snake_play(matrix, &dec);//go into snake game play to update matrix and have the snake move and for end game conditions
        }
        if(end == 2)//end game condition happened
        {
            read(matrix);//read matrix and update led accordingly
            if(d == 0)//if dec = 0
            {
                read(matrix);//read matrix and update led accordingly
                toggle_routine();//turn on LRA
                for(j = 0; j<2000000; j++);//delay
                toggle_routine();//turn off LRA
                d = 1;//d = 1
            }
            reset_matrix(matrix);//reset matrix to 0 
            game_over_matrix(matrix);//set matrix LED to game over screen
            for(i = 0; i<500; i++)//delay
            {
                read(matrix);//read matrix and update led accordingly
            }
            end = 1;//end = 1
        }
            reset_matrix(matrix);//reset matrix to 0 
            read(matrix);//read matrix and update led accordingly
            start_screen_matrix(matrix);//sets matrix start screen
            read(matrix);//read matrix and update led accordingly
            ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;//enables and starts conversions
            getDir(&DIR);//gets direction
            if(DIR != 0) sel = 1;//if direction not zero sel =1 
            if(sel == 1) end = 0;//if sel ==1 then end =0 to start a new game
    }
}
