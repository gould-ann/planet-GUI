/*
 * uart.h
 *
 *  Created on: Nov 2, 2017
 *      Author: anngould
 */

#ifndef UART_H_
#define UART_H_


void UART_init(void);
//void UART_Transmit(char sdata);
char UART_Receive(void);
void uart_sendChar(char data);
void send_string(char str[]);



#endif /* UART_H_ */
