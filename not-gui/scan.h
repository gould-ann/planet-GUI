/*
 * scan.h
 *
 *  Created on: Dec 5, 2017
 *      Author: anngould
 */

#ifndef SCAN_H_
#define SCAN_H_

/// Constant for converting prevent radians and degrees
#define PI 3.14159265

/**
 * Global variables to be used across all functions
 * buffer is the character string sent over UART
 */
char buffer[80];

#include <stdint.h>

#include <inc/tm4c123gh6pm.h>



// Resets all the variables for consistent scan readings
void scan_reset();

// Scans 180 degree arc and marks detected objects
void scan_action();


#endif /* SCAN_H_ */

