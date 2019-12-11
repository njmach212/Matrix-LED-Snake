/*
 * drv2605l.c
 *
 *  Created on: Oct 14, 2019
 *      Author: Nick
 */

#include "msp.h"
#include "i2c.h"
#include "gpio.h"
#include "drv2605l.h"
#define DRV2605L     0x5A   // 7-bit address (8.5.3.1 note)

#define STATUS       0x00
#define MODE         0x01
#define GO           0x0C
#define CONTROL3     0x1D

#define MODE_PWM     0x03
#define MODE_STANDBY (1 << 6)
#define GO_GO        0x01
#define LIBRARY      0x03

void config_drv2605L(void(*toggle_routine)()){   /*(*toggle_routine)()*/
//    uint8_t mode_reg,control_reg,library_reg;
    config_drv_gpio();   //configure EN pin
    toggle_routine();    // remove from standby mode
    int i;
    for(i = 0; i < 100000; i++);   //need to wait 250us before writing to it
    write_register(DRV2605L,MODE,MODE_STANDBY);
    uint8_t mode_reg = read_register(DRV2605L, MODE);
    if(mode_reg != (0x00)){
        P1->DIR |= BIT0;
        P1->OUT |= BIT0;
    }
    write_register(DRV2605L,MODE,MODE_PWM);
    mode_reg = read_register(DRV2605L, MODE);
    mode_reg = read_register(DRV2605L, MODE);
    if(mode_reg != (MODE_PWM)){
        P1->DIR |= BIT0;
        P1->OUT |= BIT0;
    }
    write_register(DRV2605L,CONTROL3,0x00);
    uint8_t control_reg = read_register(DRV2605L, CONTROL3);
    control_reg = read_register(DRV2605L, CONTROL3);
    if(mode_reg != (0xA1)){
        P1->DIR |= BIT0;
        P1->OUT |= BIT0;
    }
    write_register(DRV2605L,LIBRARY,0x06);
    uint8_t library_reg = read_register(DRV2605L, LIBRARY);
    library_reg = read_register(DRV2605L, LIBRARY);
    if(mode_reg != (0x06)){
        P1->DIR |= BIT0;
        P1->OUT |= BIT0;
    }

}
