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
void usart_send(volatile char*);
#endif /* USART_USART_H_ */
