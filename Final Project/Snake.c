/*
 * Snake.c
 *
 *  Created on: Dec 2, 2019
 *      Author: Nick
 */
#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "Snake.h"
#include <time.h>
#include <stdlib.h>
#include "UART.h"
#include <string.h>
#include "gpio.h"
volatile static int joy_dir;
volatile static int snake_size;

uint8_t Tail_Move[100];

typedef struct snake
{
   uint8_t head1, head2, tail1, tail2, h , t, size;
   uint8_t *tail_move;
}snake;

snake Snake;

void getDir(int *Dir)
{
    *Dir = joy_dir;
}
void put_Dir(int Dir)
{
    joy_dir = Dir;
}

int Put(snake *is,uint8_t byte)//inserts byte to circular buffer created
{
    if (((is->h + 1) % is->size) == is->t) return 1; //if input buffer front incremented modulated with
    //else                                                //input buffer size equals input buffer end then there is no where to input another byte
    is->tail_move[is->h] = byte;//set the front part of the circular buffer array to the sent byte
    is->h++;//increment front
    is->h %= is->size;//set front equal to front modulated with size to create the circular buffer
    return 0;
}

int Get(snake *rs,uint8_t *byte)//removes byte from circular buffer created
{
    if (rs->h == rs->t)
        return 1;//if front equals end then there is no byte that can be removed
    *byte = rs->tail_move[rs->t];//sets the byte sent to the byte at the end of the circular buffer
    rs->t++;//increment end
    rs->t %= rs->size;//end equals end modulated with size to create the circular buffer
    return 0;
}

void Rand_Apple(uint8_t *app1, uint8_t *app2)
{
    *app1 = rand()%30;
    *app2 = rand()%30;
    snake_size++;
    P3->OUT |= BIT0;
}

void config_snake(uint8_t matrix[32][32])
{
    int i, j;
    for(i = 0; i<32; i++)
    {
        for(j = 0; j<32; j++)
        {
            if(j == 0 || i == 0 || i == 31 || j == 31) matrix[i][j] = 2;
            else matrix[i][j] = 0;
        }
    }
    matrix[16][16] = 2;
    matrix[16][25] = 1;
    snake_size = 1;
    Snake.tail1 = 16;
    Snake.head1 = 16;
    Snake.tail2 = 16;
    Snake.head2 = 16;
    Snake.tail_move = Tail_Move;
    Snake.h = 0;
    Snake.t = 0;
    Snake.size = 100;
}

int snake_play(uint8_t matrix[32][32], int *Dec)
{
    uint8_t byte, app1 = Snake.head1, app2 = Snake.head2;
    int Dir = 0, dec = 0, u = 0, d = 0, l = 0, r = 0, end = 0;
    int i, j, I = 0, J = 0;
    getDir(&Dir);
    for(i = 0; i<32; i++)
    {
        for(j = 0; j<32; j++)
        {
            if(Dir == 1 && matrix[i][j] == 2 && Snake.head1 == i && Snake.head2 == j)
            {
                Put(&Snake, 1);
                l = 1;
                I = i;
                J = j;
                if(matrix[i][j-1] == 1)
                {
                    dec = 1;
                    while(matrix[app1][app2] == 2)
                    {
                    Rand_Apple(&app1, &app2);
                    }
                    matrix[app1][app2] = 1;
                }
                if(matrix[i][j-1] == 2) end = 1;
            }
            if(Dir == 2 && matrix[i][j] == 2 && Snake.head1 == i && Snake.head2 == j)
            {
                Put(&Snake, 2);
                r = 1;
                I = i;
                J = j;
                if(matrix[i][j+1] == 1)
                    {
                        dec = 1;
                        while(matrix[app1][app2] == 2)
                        {
                        Rand_Apple(&app1, &app2);
                        }
                        matrix[app1][app2] = 1;
                    }
                if(matrix[i][j+1] == 2) end = 1;
            }
            if(Dir == 3 && matrix[i][j] == 2 && Snake.head1 == i && Snake.head2 == j)
            {
                Put(&Snake, 3);
                u = 1;
                I = i;
                J = j;
                if(matrix[i-1][j] == 1)
                    {
                        dec = 1;
                        while(matrix[app1][app2] == 2)
                        {
                        Rand_Apple(&app1, &app2);
                        }
                        matrix[app1][app2] = 1;
                    }
                if(matrix[i-1][j] == 2) end = 1;
            }
            if(Dir == 4 && matrix[i][j] == 2 && Snake.head1 == i && Snake.head2 == j)
            {
                Put(&Snake, 4);
                d = 1;
                I = i;
                J = j;
                if(matrix[i+1][j] == 1)
                    {
                        dec = 1;
                        while(matrix[app1][app2] == 2)
                        {
                        Rand_Apple(&app1, &app2);
                        }
                        matrix[app1][app2] = 1;
                    }
                if(matrix[i+1][j] == 2) end = 1;
            }
            if(Snake.head1 == i && Snake.head2 == j && dec == 0)
            {
               Get(&Snake,&byte);
               if(byte == 1)
               {
                   matrix[Snake.tail1][Snake.tail2] = 0;
                   Snake.tail1 = Snake.tail1;
                   Snake.tail2 = Snake.tail2-1;
               }
                if(byte == 2)
                {
                    matrix[Snake.tail1][Snake.tail2] = 0;
                    Snake.tail1 = Snake.tail1;
                    Snake.tail2 = Snake.tail2+1;
                }
                if(byte == 3)
                {
                    matrix[Snake.tail1][Snake.tail2] = 0;
                    Snake.tail1 = Snake.tail1-1;
                    Snake.tail2 = Snake.tail2;
                }
                if(byte == 4)
                {
                    matrix[Snake.tail1][Snake.tail2] = 0;
                    Snake.tail1 = Snake.tail1+1;
                    Snake.tail2 = Snake.tail2;
                }
            }
        }
    }
    *Dec = dec;
    /*if(dec == 1)
    {
        for(i = 0; i<2000000; i++);
            toggle_routine();
    }*/
    if(end == 1)
    {
        joy_dir = 0;
        snake_size = 0;
        return 2;
    }
    else if(l == 1)
    {
        Snake.head1 = I;
        Snake.head2 = J-1;
        matrix[I][J-1] = 2;
    }
    else if(r == 1)
    {
        Snake.head1 = I;
        Snake.head2 = J+1;
        matrix[I][J+1] = 2;
    }
    else if(u == 1)
    {
        Snake.head1 = I-1;
        Snake.head2 = J;
        matrix[I-1][J] = 2;
    }
    else if(d == 1)
    {
        Snake.head1 = I+1;
        Snake.head2 = J;
        matrix[I+1][J] = 2;
    }
    int k,p;
    char buf1[16];
    char counter[] = "Snake Size:";
    char clear[] = "|-";
    p = snake_size;
    k = sprintf((char *)buf1,"%d",(const char *)p);
    send_string((uint8_t *) clear,strlen(clear));
    send_string((uint8_t *) counter,strlen(counter));
    send_string((uint8_t *) buf1,k);
    return 0;
}


void ADC14_GPIO_CONFIG(void)
{
    //P5.0 and P5.2 -> ADC function

    P5->SEL1 |= BIT0;
    P5->SEL1 |= BIT2;
    P5->SEL0 |= BIT0;
    P5->SEL0 |= BIT2;
}

void ADC14_INIT(void)
{
    //Enable Interrupts


    NVIC_SetPriority(ADC14_IRQn, 2);
    NVIC_EnableIRQ(ADC14_IRQn);

    //Configure ADC14

    ADC14->CTL0 = (ADC14_CTL0_SHT1__192 | ADC14_CTL0_SHP | ADC14_CTL0_ON);
    ADC14->CTL1 = ADC14_CTL1_RES__12BIT;

    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_3;
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_5;
    ADC14->MCTL[1] |= ADC14_MCTLN_EOS;

    ADC14->IER0 |= ADC14_IER0_IE1;
    ADC14->IER0 |= ADC14_IER0_IE0;

    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_3;

    //Start Conversion

    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
}

void ADC14_IRQHandler(void)
{
    // Handle IRQs in ADC14IV

    uint16_t results;

    if (ADC14->IFGR0 & ADC14_CLRIFGR0_CLRIFG1)
    {
      // ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG1;

      results = (uint16_t) ADC14->MEM[1];  // reading MEM1 clears associated interrupt flag

      if (0x0FA0 <= results) joy_dir = DOWN;
      else if (0x0064 >= results) joy_dir = UP;
    }
    if (ADC14->IFGR0 & ADC14_CLRIFGR0_CLRIFG0)
    {
      // ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0;

      results = (uint16_t) ADC14->MEM[0];  // reading MEM0 clears associated interrupt flag

      if (0x0FA0 <= results) joy_dir = LEFT;
      else if (0x0064 >= results) joy_dir = RIGHT;
    }
}
