/*
 * usart.c
 *
 *  Created on: Apr 25, 2016
 *      Author: eason
 */
#include "usart.h"

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "misc.h" //for nvic stuff
#include <stdio.h>
#include "circarray/circarray.h"



#define MAX_STRLEN 50
CircArr_InitTypeDef msg; //first create 1 circular array buffer called msg
//make above static?

//buf_test(&msg);

/**@fn void init_usart1(uint32_t baud)
 * @brief Configures and initializes USART and its GPIO and interrupt controller
 * @param baud takes baud rate
 * @retval none
 */
void init_usart1(uint32_t baud)
{
	if (!baud) {
		baud = 9600;
		printf("Warning! No baud set for usart1! (setting to 9600bps\n");
	}

	//our structs used for initialization
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;


	// Enable clock for GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//Enable clock for usart 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


	//Tell pins PB6 and PB7 which alternating function you will use
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); //tx
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1); //rx

	// Initialize gpio pins with alternating function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;   // = TX | RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);


	//initialize usart
	USART_InitStruct.USART_BaudRate = baud; // default is 9600 baud
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;  //8 bit size
	USART_InitStruct.USART_StopBits = USART_StopBits_1;  //1 stop bit
	USART_InitStruct.USART_Parity = USART_Parity_No; //no parity
	USART_InitStruct.USART_HardwareFlowControl =USART_HardwareFlowControl_None;  //no flow control
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //enable two way communication
	USART_Init(USART1, &USART_InitStruct);


	//(Nested Vector Interrupt controller)
	//config receive interrupt so that we can get messages as soon as they come in
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //enable usart1 receive interrupt
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn; //attach usart1's irq
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; //interrupt priority group (high)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; //sub priority within group (high)
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//initializes a circular array for storing incoming serial data
	initCircArray(&msg,200);


	//Lastly, enable usart1
	USART_Cmd(USART1, ENABLE);


}

//
/**@fn void USART1_IRQHandler()
 * @brief Overload of usart interrupt handler. receives incoming chars and
 * collects them into a circular array buffer and prints them to debug terminal
 * afterwards. overloads startup irqhandler
 * @param ascii char
 * @retval none
 */
void USART1_IRQHandler()
{

if (USART_GetITStatus(USART1, USART_IT_RXNE))
		{
	      //get newest incoming char/byte from data register and put in buffer
			char c = USART1->DR;

			buf_putbyte(&msg,c);

		}

}
/**
 * @brief Function to transmit a string via USART1 (work in progress)
 * @param ascii char
 * @retval none
 */
void usart1_send( volatile char *s)
{

	while(*s) {
		while ( !(USART1->SR & 0x00000040)); //wait for empty data register
					USART_SendData(USART1, *s); //send char
					*s++;
	}
}
/**
 * @brief Function returns next unread byte from serial buffer
 * @param none
 * @retval uint8_t
 */
uint8_t usart1_read(void){
	return buf_getbyte(&msg);
}

/**
 * @brief Function returns next unread byte as a char from serial buffer
 * @param none
 * @retval uint8_t
 */
char usart1_readc(void){
	return (char)buf_getbyte(&msg);
}

/**
 * @brief Function returns number of bytes waiting to be read in serial buffer
 * @param none
 * @retval uint8_t
 */
uint8_t usart1_available(void) {
	return buf_available(&msg);
}



