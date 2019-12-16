/*
 * matrixLED.c
 *
 *  Created on: Nov 25, 2019
 *      Author: Nick and Maurice
 */
#include "matrixLED.h"
#include <stdio.h>
#include "msp.h"
#include<stdint.h>

// zeroes out a matrix
void reset_matrix(uint8_t matrix[32][32])
{
    int i, j;
    for(i = 0; i<32; i++)
    {
        for(j = 0; j<32; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

// sets address pins to input value adr
void address(uint8_t adr)
{
    if(adr==0 || adr==16)
    {
        P4->OUT &= ~BIT1;   // A
        P6->OUT &= ~BIT0;   // B
        P4->OUT &= ~BIT0;   // C
        P5->OUT &= ~BIT4;   // D
    }
    if(adr==1 || adr == 17)
    {
        P4->OUT |= BIT1;   // A
        P6->OUT &= ~BIT0;   // B
        P4->OUT &= ~BIT0;   // C
        P5->OUT &= ~BIT4;   // D
    }
    if(adr==2 || adr==18)
    {
        P4->OUT &= ~BIT1;   // A
        P6->OUT |= BIT0;   // B
        P4->OUT &= ~BIT0;   // C
        P5->OUT &= ~BIT4;   // D
    }
    if(adr==3 || adr==19)
    {
        P4->OUT |= BIT1;   // A
        P6->OUT |= BIT0;   // B
        P4->OUT &= ~BIT0;   // C
        P5->OUT &= ~BIT4;   // D
    }
    if(adr==4 || adr==20)
    {
        P4->OUT &= ~BIT1;   // A
        P6->OUT &= ~BIT0;   // B
        P4->OUT |= BIT0;   // C
        P5->OUT &= ~BIT4;   // D
    }
    if(adr==5 || adr==21)
    {
        P4->OUT |= BIT1;   // A
        P6->OUT &= ~BIT0;   // B
        P4->OUT |= BIT0;   // C
        P5->OUT &= ~BIT4;   // D
    }
    if(adr==6 || adr==22)
    {
        P4->OUT &= ~BIT1;   // A
        P6->OUT |= BIT0;   // B
        P4->OUT |= BIT0;   // C
        P5->OUT &= ~BIT4;   // D
    }
    if(adr==7 || adr==23)
    {
        P4->OUT |= BIT1;   // A
        P6->OUT |= BIT0;   // B
        P4->OUT |= BIT0;   // C
        P5->OUT &= ~BIT4;   // D
    }
    if(adr==8 || adr == 24)
    {
        P4->OUT &= ~BIT1;   // A
        P6->OUT &= ~BIT0;   // B
        P4->OUT &= ~BIT0;   // C
        P5->OUT |= BIT4;   // D
    }
    if(adr==9 || adr==25)
    {
        P4->OUT |= BIT1;   // A
        P6->OUT &= ~BIT0;   // B
        P4->OUT &= ~BIT0;   // C
        P5->OUT |= BIT4;   // D
    }
    if(adr==10 || adr==26)
    {
        P4->OUT &= ~BIT1;   // A
        P6->OUT |= BIT0;   // B
        P4->OUT &= ~BIT0;   // C
        P5->OUT |= BIT4;   // D
    }
    if(adr==11 || adr==27)
    {
        P4->OUT |= BIT1;   // A
        P6->OUT |= BIT0;   // B
        P4->OUT &= ~BIT0;   // C
        P5->OUT |= BIT4;   // D
    }
    if(adr==12 || adr==28)
    {
        P4->OUT &= ~BIT1;   // A
        P6->OUT &= ~BIT0;   // B
        P4->OUT |= BIT0;   // C
        P5->OUT |= BIT4;   // D
    }
    if(adr==13 || adr==29)
    {
        P4->OUT |= BIT1;   // A
        P6->OUT &= ~BIT0;   // B
        P4->OUT |= BIT0;   // C
        P5->OUT |= BIT4;   // D
    }
    if(adr==14 || adr==30)
    {
        P4->OUT &= ~BIT1;   // A
        P6->OUT |= BIT0;   // B
        P4->OUT |= BIT0;   // C
        P5->OUT |= BIT4;   // D
    }
    if(adr==15 || adr==31)
    {
        P4->OUT |= BIT1;   // A
        P6->OUT |= BIT0;   // B
        P4->OUT |= BIT0;   // C
        P5->OUT |= BIT4;   // D
    }
}

// reads values of given matrix and turns LEDs on based on the matrix values
void read(uint8_t matrix[32][32])
{
    int i, j, adr = 0, I;
    for(i=0; i<32; i++)
    {
        for(j=0; j<32; j++)
        {
            P1->OUT &= ~BIT5;   // r2
            P4->OUT &= ~BIT6;   // b2
            P4->OUT &= ~BIT5;   // g2
            P5->OUT &= ~BIT5;   // r1
            P4->OUT &= ~BIT2;   // b1
            P4->OUT &= ~BIT4;   // g1
            if(matrix[i][j] == 2) // if a snake, turn that LED green
            {
                if(i<16)
                {
                    P5->OUT &= ~BIT5;   // r1
                    P4->OUT &= ~BIT2;   // b1
                    P4->OUT |= BIT4;   // g1
                }
                else
                {
                    P1->OUT &= ~BIT5;   // r2
                    P4->OUT &= ~BIT6;   // b2
                    P4->OUT |= BIT5;   // g2
                }
            }
            else if(matrix[i][j] == 1) // if a apple turn that LED red
            {
                if(i<16)
                {
                    P5->OUT |= BIT5;   // r1
                    P4->OUT &= ~BIT2;   // b1
                    P4->OUT &= ~BIT4;   // g1
                }
                else
                {
                    P1->OUT |= BIT5;   // r2
                    P4->OUT &= ~BIT6;   // b2
                    P4->OUT &= ~BIT5;   // g2
                }
            }
            else if(matrix[i][j] == 0) // if zero, then turn LED off
            {
                if(i<16)
                {
                    P5->OUT &= ~BIT5;   // r1
                    P4->OUT &= ~BIT2;   // b1
                    P4->OUT &= ~BIT4;   // g1
                }
                else
                {
                    P1->OUT &= ~BIT5;   // r2
                    P4->OUT &= ~BIT6;   // b2
                    P4->OUT &= ~BIT5;   // g2
                }
            }
            else if(matrix[i][j] == 3) // if matrix value is 3, turn LED blue
            {
                if(i<16)
                {
                    P5->OUT &= ~BIT5;   // r1
                    P4->OUT |= BIT2;   // b1
                    P4->OUT &= ~BIT4;   // g1
                }
                else
                {
                    P1->OUT &= ~BIT5;   // r2
                    P4->OUT |= BIT6;   // b2
                    P4->OUT &= ~BIT5;   // g2
                }
            }
            P4->OUT ^= BIT3; // toggle CLK to clock out RGB data to LED drivers on matrix LED
            P4->OUT ^= BIT3;
        }
        P6->OUT |= BIT5; // toggle LAT, capture data
        P6->OUT &= ~BIT4; // toggle OE, enable outputs
        for(I = 0; I<50; I++);
        P6->OUT |= BIT4; // OE disable outputs
        address(adr);
        P6->OUT &= ~BIT5; // LAT pass through data
        adr++; // increment to next address
        if(adr == 32) adr = 0; // once address reaches 32, set back to zero
    }

}

// configures the pins connected to matrix LED to outputs
void config_matrix_led(void)
{
    P5->DIR |= BIT5; // R1  *
    P4->DIR |= BIT2; // B1  *
    P4->DIR |= BIT4; // G1  *

    P1->DIR |= BIT5; // R2  *
    P4->DIR |= BIT6; // B2  *
    P4->DIR |= BIT5; // G2  *

    P4->DIR |= BIT1; // A   *
    P6->DIR |= BIT0; // B   *
    P4->DIR |= BIT0; // C   *
    P5->DIR |= BIT4; // D   *

    P6->DIR |= BIT4; // OE  *
    P6->DIR |= BIT5; // LAT *
    P4->DIR |= BIT3; // CLK *
}

// creates a matrix that displays snake
void start_screen_matrix(uint8_t M[32][32]) {
    // S
                                           M[3][4] = 2; M[3][5] = 2; M[3][6] = 2; M[3][7] = 2;
                                           M[4][4] = 2;                           M[4][7] = 2;
                                           M[5][4] = 2;                           M[5][7] = 2;
    M[6][1] = 2;                           M[6][4] = 2;
    M[7][1] = 2;                           M[7][4] = 2;
    M[8][1] = 2; M[8][2] = 2; M[8][3] = 2; M[8][4] = 2;
    // N
    M[3][9] = 2;                                           M[3][13] = 2;
    M[4][9] = 2; M[4][10] = 2;                             M[4][13] = 2;
    M[5][9] = 2;               M[5][11] = 2;               M[5][13] = 2;
    M[6][9] = 2;                             M[6][12] = 2; M[6][13] = 2;
    M[7][9] = 2;                                           M[7][13] = 2;
    M[8][9] = 2;                                           M[8][13] = 2;
    // A
                                M[3][17] = 2;
                  M[4][16] = 2;               M[4][18] = 2;
    M[5][15] = 2;                                           M[5][19] = 2;
    M[6][15] = 2; M[6][16] = 2; M[6][17] = 2; M[6][18] = 2; M[6][19] = 2;
    M[7][15] = 2;                                           M[7][19] = 2;
    M[8][15] = 2;                                           M[8][19] = 2;
    // K
    M[3][21] = 2;                             M[3][24] = 2;
    M[4][21] = 2;               M[4][23] = 2;
    M[5][21] = 2; M[5][22] = 2;
    M[6][21] = 2;               M[6][23] = 2;
    M[7][21] = 2;                             M[7][24] = 2;
    M[8][21] = 2;                             M[8][24] = 2;
    // E
    M[3][26] = 2; M[3][27] = 2; M[3][28] = 2; M[3][29] = 2;
    M[4][26] = 2;
    M[5][26] = 2; M[5][27] = 2; M[5][28] = 2; M[5][29] = 2;
    M[6][26] = 2;
    M[7][26] = 2;
    M[8][26] = 2; M[8][27] = 2; M[8][28] = 2; M[8][29] = 2;
}

// creates a matrix that displays "game over"
void game_over_matrix(uint8_t M[32][32]) {
    // G
                              M[3][4] = 2; M[3][5] = 2; M[3][6] = 2;
                 M[4][3] = 2;
    M[5][2] = 2;
    M[6][2] = 2;                           M[6][5] = 2; M[6][6] = 2; M[6][7] = 2;
                 M[7][3] = 2;                           M[7][6] = 2;
                              M[8][4] = 2; M[8][5] = 2;
    // A
                               M[3][11] = 2;
                 M[4][10] = 2;               M[4][12] = 2;
    M[5][9] = 2;                                           M[5][13] = 2;
    M[6][9] = 2; M[6][10] = 2; M[6][11] = 2; M[6][12] = 2; M[6][13] = 2;
    M[7][9] = 2;                                           M[7][13] = 2;
    M[8][9] = 2;                                           M[8][13] = 2;
    // M
    M[3][15] = 2;                                           M[3][19] = 2;
    M[4][15] = 2; M[4][16] = 2;               M[4][18] = 2; M[4][19] = 2;
    M[5][15] = 2;               M[5][17] = 2;               M[5][19] = 2;
    M[6][15] = 2;                                           M[6][19] = 2;
    M[7][15] = 2;                                           M[7][19] = 2;
    M[8][15] = 2;                                           M[8][19] = 2;
    // E
    M[3][21] = 2; M[3][22] = 2; M[3][23] = 2; M[3][24] = 2;
    M[4][21] = 2;
    M[5][21] = 2; M[5][22] = 2; M[5][23] = 2; M[5][24] = 2;
    M[6][21] = 2;
    M[7][21] = 2;
    M[8][21] = 2; M[8][22] = 2; M[8][23] = 2; M[8][24] = 2;
    // O
                  M[10][3] = 2; M[10][4] = 2; M[10][5] = 2;
    M[11][2] = 2;                                           M[11][6] = 2;
    M[12][2] = 2;                                           M[12][6] = 2;
    M[13][2] = 2;                                           M[13][6] = 2;
    M[14][2] = 2;                                           M[14][6] = 2;
                  M[15][3] = 2; M[15][4] = 2; M[15][5] = 2;
   // V
   M[10][8] = 2;                                             M[10][12] = 2;
   M[11][8] = 2;                                             M[11][12] = 2;
   M[12][8] = 2;                                             M[12][12] = 2;
   M[13][8] = 2;                                             M[13][12] = 2;
                 M[14][9] = 2;                M[14][11] = 2;
                               M[15][10] = 2;
   // E
   M[10][14] = 2; M[10][15] = 2; M[10][16] = 2; M[10][17] = 2;
   M[11][14] = 2;
   M[12][14] = 2; M[12][15] = 2; M[12][16] = 2; M[12][17] = 2;
   M[13][14] = 2;
   M[14][14] = 2;
   M[15][14] = 2; M[15][15] = 2; M[15][16] = 2; M[15][17] = 2;
   // R
   M[10][19] = 2; M[10][20] = 2; M[10][21] = 2;
   M[11][19] = 2;                               M[11][22] = 2;
   M[12][19] = 2; M[12][20] = 2; M[12][21] = 2;
   M[13][19] = 2;                M[13][21] = 2;
   M[14][19] = 2;                               M[14][22] = 2;
   M[15][19] = 2;                               M[15][22] = 2;
}

// function to test matrix LED functionality
void test(void)
{
    P5->OUT &= ~BIT5;   // r1
    P4->OUT |= BIT2;   // b1
    P4->OUT |= BIT4;   // g1

    P1->OUT |= BIT5;   // r2
    P4->OUT &= ~BIT6;   // b2
    P4->OUT &= ~BIT5;   // g2

    P6->OUT |= BIT4;    // ~OE
    P6->OUT &= ~BIT5;    // LAT
    P4->OUT &= ~BIT3; //CLK

    int i, j;
    uint8_t adr = 0;
    while(1)
    {
        for(i = 0; i < 32; i++)
        {
            adr = i;
            address(adr);
            P4->OUT ^= BIT3;
            P4->OUT ^= BIT3;
            for(j = 0; j<32; j++)
            {
                if(j%2 == 0)
                {
                    P5->OUT |= BIT5;   // r1
                    P4->OUT |= BIT2;   // b1
                    P4->OUT |= BIT4;   // g1
                }
                else
                {
                    P5->OUT &= ~BIT5;   // r1
                    P4->OUT &= ~BIT2;   // b1
                    P4->OUT &= ~BIT4;   // g1
                }
                P4->OUT ^= BIT3;
                P4->OUT ^= BIT3;
            }
            P6->OUT |= BIT5; // toggle LAT, capture data
            P6->OUT &= ~BIT4; // toggle OE, enable outputs
            P6->OUT |= BIT4; // OE disable outputs
            P6->OUT &= ~BIT5; // LAT pass through data
        }
    }
}
