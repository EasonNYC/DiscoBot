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




#define MAX_STRLEN 50
volatile char rx_buff[MAX_STRLEN+1]; //plus room for null terminator


/**
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


	//Lastly, enable usart1
	USART_Cmd(USART1, ENABLE);


}

//receives incoming chars and collects them into an array and prints them to debug terminal afterwards. overwrites startup irqhandler
void USART1_IRQHandler()
{

if (USART_GetITStatus(USART1, USART_IT_RXNE))
		{
	      //get newest char
			char c = USART1->DR;
			static uint8_t charcount = 0;

			//make sure its not the return char and make sure our incoming string is not too big
			if((c != '\n') && (charcount < MAX_STRLEN))
			{
				rx_buff[charcount] = c; //add char to received message array
			   charcount++;
			}
			else
			{
			 rx_buff[charcount] = '\n';
			 charcount = 0;
			 //usart_send(USART1, rx_buff);  //transmit/echo back the received string via usart
			 printf("rec: ");
			 printf("%s", rx_buff); //print for testing

			 //now rx_buff contains a full string message
			 //and we can copy it/send it anywhere in our robots code

			 //todo: on receive set flag?
			}
		}

}
/**
 * @brief Function to transmit a string via USART1 (work in progress)
 * @param ascii char
 * @retval none
 */
void usart_send( volatile char *s)
{

	while(*s) {
		while ( !(USART1->SR & 0x00000040)); //wait for empty data register
					USART_SendData(USART1, *s); //send char
					*s++;
	}
}



/*
 *send 1 char
int putcharx(int ch){
	while (USART_GetFLagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, (uint8_t)ch);
}
*/

