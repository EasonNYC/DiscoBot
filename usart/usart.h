/*
 * usart.h
 *
 *  Created on: Apr 25, 2016
 *      Author: eason
 */

#ifndef USART_USART_H_
#define USART_USART_H_

#include <stdint.h>

void init_usart1(uint32_t);
void usart1_send(volatile char*);
uint8_t usart1_read(void);
char usart_readc(void);
uint8_t usart1_available(void);
#endif /* USART_USART_H_ */
