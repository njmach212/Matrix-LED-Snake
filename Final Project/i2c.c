/*
 * 12c.c
 *
 *  Created on: Oct 14, 2019
 *      Author: Jack
 */

#include "msp.h"
#include <stdint.h>
#include "i2c.h"

/*
 * P1.6 --> SDA
 * P1.7 --> SCL
 * EUSCI_B0
 */
//Jack made this for us in a previous lab
void config_i2c(void) {
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;  // UCSWRST = 1

    // configure I2C
    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_MODE_3 |   // select I2C mode
                        EUSCI_B_CTLW0_MST    |   // select master mode
                        EUSCI_B_CTLW0_UCSSEL_3);    // select SMCLK as clock source

    EUSCI_B0->BRW = 8;  // set prescaler to have 375kHz SCL line

    // Configure port
    P1->SEL0 |= (BIT6 | BIT7);
    P1->SEL1 &= ~(BIT6 | BIT7);


    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);  // clear UCSWRST

//    EUSCI_B0->IE |= (EUSCI_B_IE_TXIE0 | EUSCI_B_IE_RXIE0);
    EUSCI_B0->IFG = 0;
//    NVIC_EnableIRQ(EUSCIB0_IRQn);  // enable interrupts
}

void write_register(uint8_t address, uint8_t reg, uint8_t value) {
    set_as_transmitter();       // set to transmit mode
    set_i2c_address(address);   // set slave address
    set_i2c_byte_counter(2);
    send_start();               // start communication

    EUSCI_B0->TXBUF = reg;  // send register being written to
    while(!(EUSCI_B0->IFG  & EUSCI_B_IFG_TXIFG0));

    EUSCI_B0->TXBUF = value;  // send value being written to
    while(!(EUSCI_B0->IFG  & EUSCI_B_IFG_TXIFG0));

    send_stop();
}


uint8_t read_register(uint8_t address, uint8_t reg) {
    // if bus is busy return 0xFF
    if(EUSCI_B0->STATW & EUSCI_B_STATW_BBUSY) {
        return 0xFF;
    }

    else {  // try reading over I2C protocol
        set_as_transmitter();       // set to transmit mode
        set_i2c_address(address);   // set slave address
        set_i2c_byte_counter(1);
        send_start();               // start communication

        EUSCI_B0->TXBUF = reg;  // send register being written to
        while(!(EUSCI_B0->IFG  & EUSCI_B_IFG_TXIFG0));

        set_i2c_byte_counter(1);
        set_as_receiver();
        set_i2c_address(address);   // set slave address
        send_start();           // send repeated start

        uint8_t rx_data = 0xFF;

        // wait for data, bad because of polling :(
        while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));
        rx_data = EUSCI_B0->RXBUF;

        send_stop();    // compete communication
        return rx_data;
    }
}

void send_start(void) {
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;  // send start
    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);  // wait for start and Address to be sent
}


void send_stop(void) {
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;  // send stop
    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);  // wait until stop is sent
}

void set_as_transmitter(void) {
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;  // set as Transmitter
}

void set_as_receiver(void) {
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_TR;  // set as Transmitter
}

/* change the slave address being communicated with */
void set_i2c_address(uint8_t addr) {
    EUSCI_B0->I2CSA = (addr);
}

void set_i2c_byte_counter(uint8_t n) {
    EUSCI_B0->TBCNT = n;
}

//void EUSCIB0_IRQHandler(void) {
//    if(EUSCI_B0->IFG & )
//}
