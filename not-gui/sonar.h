/*
 * sonar.h
 *
 *  Created on: Oct 24, 2017
 *      Author: anngould
 */



#ifndef SONAR_H_

#define SONAR_H_
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "timer.h"

#include "lcd.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

void ping_init();
unsigned ping_read();
void TIMER3B_Handler(void);
void send_pulse();
unsigned time2dist(unsigned);
void clock_timer_init(void);

#endif /* SONAR_H_ */
