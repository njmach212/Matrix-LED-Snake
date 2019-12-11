/*
 * UART.c
 *
 *  Created on: Nov 10, 2019
 *      Author: Nick
 */

#include "msp.h"
#include <stdint.h>
#include "UART.h"
#include "string.h"
//P3.3 TX
//P3.2 RX

#define RxBUFR_MAX 32//defines max length of the RX and TX buffers
#define TxBUFR_MAX 32

uint8_t RxBufr[RxBUFR_MAX];//defines 2 arrays with length of the max length of the TX and RX buffers
uint8_t TxBufr[TxBUFR_MAX];

typedef struct bufr_t  //creates a structure called bufr_t that is a circular buffer
{
    uint8_t front,end; //bufr_t has a front and an end which indicate the location of the front and end of the circular buffer
    uint8_t *bufr;  //bufr_t has a pointer to an array such as TxBufr and RxBufr above
    uint8_t size;  //bufr_t has a max size of the buffer

} bufr_t;

bufr_t rx,tx; //defines 2 bufr_t structures one for rx and one for tx

void config_UART(void)
{
    rx.bufr = RxBufr;//the pointer bufr in bufr_t rx points to the array RxBufr
    rx.front = rx.end = 0;//initializes the front and end of rx to 0
    rx.size = RxBUFR_MAX;//creates the max size of rx to be the max size of RxBufr
    tx.bufr = TxBufr;//the pointer bufr in bufr_t tx points to the array TxBufr
    tx.front = tx.end = 0;//initializes the front and end of tx to 0
    tx.size = TxBUFR_MAX;//creates the max size of tx to be the max size of TxBuf

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;  // UCSWRST = 1 software reset enabled
    EUSCI_A2->CTLW0 |= (EUSCI_A_CTLW0_MODE_0   |   // select UART mode
                        EUSCI_A_CTLW0_UCSSEL_2 );    // select SMCLK as clock source
    // Configure port
    P3->DIR |= BIT3;//sets transmit to output and receice to input
    P3->DIR &= ~BIT2;

    P3->SEL0 |= (BIT2 | BIT3);//selects UART m tx rx for P3.3 and P3.2
    P3->SEL1 &= ~(BIT2 | BIT3);

    //EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_PEN; //enable parity
    //EUSCI_A2->CTLW0 |= BIT14; //set parity to even
    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_MSB;//sets least significant bit to be sent first
    EUSCI_A2->CTLW0 &= ~EUSCI_B_CTLW0_SEVENBIT;//sets to receive 8 bit character length
    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SPB;//sets the number of stop bits to 1
    EUSCI_A2->CTLW0 &= ~BIT8;//sets it to asynchronous mode
    //baud rate calculation
    //N = 3000000/(9600) = 312.5 N>16
    //OS16 = 1
    //UCBRx = INT(N/16) = 19
    //UCBRFx = INT([(N/16) - (INT(N/16)] * 16) = 0x08
    //UBRSx = 0xAA
    EUSCI_A2->BRW = 19; //divides bit clock so 18 cycles for every 1 cycle of baud rate clock
    EUSCI_A2->MCTLW = 0xAA81;//sets UCBRSx first, then UCBRFx next, and OS16 last

    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);// clear UCSWRST
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;// enables transmit interrupt
    NVIC_SetPriority(EUSCIA2_IRQn, 8);// sets interrupt priority
    NVIC_EnableIRQ(EUSCIA2_IRQn);// enables IRQ handler goes to handler when txFlg/rxFlg is high
}

int insert_byte(bufr_t *ib,uint8_t byte)//inserts byte to circular buffer created
{
    if (((ib->front + 1) % ib->size) == ib->end) return 1; //if input buffer front incremented modulated with
    //else                                                //input buffer size equals input buffer end then there is no where to input another byte
    ib->bufr[ib->front] = byte;//set the front part of the circular buffer array to the sent byte
    ib->front++;//increment front
    ib->front %= ib->size;//set front equal to front modulated with size to create the circular buffer
    return 0;
}

int remove_byte(bufr_t *ib,uint8_t *byte)//removes byte from circular buffer created
{
    if (ib->front == ib->end)
        return 1;//if front equals end then there is no byte that can be removed
    *byte = ib->bufr[ib->end];//sets the byte sent to the byte at the end of the circular buffer
    ib->end++;//increment end
    ib->end %= ib->size;//end equals end modulated with size to create the circular buffer
    return 0;
}

void EUSCIA2_IRQHandler(void)//interrupt handler
{
    uint8_t byte;//create a byte

    if(EUSCI_A_IFG_TXIFG & EUSCI_A2->IFG)//if tx flag bit is set and register is set
    {
        if (0 == remove_byte(&tx,&byte))//if remove byte was successful ie could remove byte
        {
            EUSCI_A2->TXBUF = byte;//set TXBUF equal to the byte removed
            EUSCI_A2->IFG &= ~EUSCI_A_IFG_TXIFG;//turn off the flag
        }
        else EUSCI_A2->IE &= ~EUSCI_A_IE_TXIE; // disables transmit interrupt
    }
    if(EUSCI_A_IFG_RXIFG & EUSCI_A2->IFG)//if rx flag bit is set and register set
    {
        insert_byte(&rx,EUSCI_A2->RXBUF);//insert byte from RXBUF to rx circular buffer
        EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;//turn off rx flag
    }
}

int send_string(uint8_t *str,uint16_t length)//sends string over UARt
{
    int i;//int i

    for(i = 0; i<length; i++)//for loop until i is less than the length of string
    {
        NVIC_DisableIRQ(EUSCIA2_IRQn);
        if (insert_byte(&tx,str[i])) break;//if insert byte fails ie nothing could be inserted break the for loop
        NVIC_EnableIRQ(EUSCIA2_IRQn);
    }
    EUSCI_A2->IE |= EUSCI_A_IE_TXIE; // enables transmit interrupt because there is a spot for a byte to be inserted
    NVIC_EnableIRQ(EUSCIA2_IRQn);
    if (i != length) return i;//if i is less than length return i so the user knows how many bytes were sent
    return length;//else return length meaning all the bytes were sent
}

int receive_string(uint8_t *str,uint16_t size)//receive string over UARt
{
    int check=1,i=0;//int i
    uint8_t byte;//create byte

    while (check)
    {
        NVIC_DisableIRQ(EUSCIA2_IRQn);
        check = remove_byte(&rx,&byte);//while you can remove remove byte and put it into rx circular buffer
        NVIC_EnableIRQ(EUSCIA2_IRQn);
        if (check) break;
        str[i++] = byte;//increment string and set it equal to byte so that it gets the value of the removed byte from the circular buffer
        if (i >= size) return size;//if i is greater than or equal to size return size
    }
    return i;//else return i
}
