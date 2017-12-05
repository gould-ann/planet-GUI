/*
 * adc_init.c
 *
 *  Created on: Oct 11, 2017
 *      Author: anngould
 */

#include "Timer.h"
#include <stdint.h>
#include "adc_init.h"
#include <math.h>



void init_ADC(){
    //init ADC, start with GPIO
    //pin 4 port B
    //enable ADC 0 module on port D
    //SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //enable clock for ADC
    SYSCTL_RCGCADC_R |= 0x1;
    //enable port D pin 0 to work as alternate functions
    GPIO_PORTB_AFSEL_R |= 0x10;
    //set pin to input - 0
    //den means 0s are enable
    GPIO_PORTB_DEN_R &= 0b11110111;
    //disable analog isolation for the pin
    GPIO_PORTB_AMSEL_R |= 0x10;
    //initialize the port trigger source as processor (default)
    GPIO_PORTB_ADCCTL_R = 0x00;
    //init ADC, end with ADC
    //disable SS0 sample sequencer to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
    //initialize the ADC trigger source as processor (default)
    ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
    //set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX0_R |= 0x000A;
    //enable raw interrupt status
    ADC0_SSCTL0_R |= (ADC_SSCTL1_IE0 | ADC_SSCTL1_END0);
    //enable oversampling to average
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    //re-enable ADC0 SS0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;

}

int ir_stuff() {
    //pulls data from ADC
           ADC0_PSSI_R = ADC_PSSI_SS0; //alternate function that we put data into

           //from textbook/notes
           while(ADC0_RIS_R & ADC_RIS_INR0 == 0){
           }
           //ADC0_ISC_R = ADC_ISC_IN0;
           int save = ADC0_SSFIFO0_R;
          // roomba 5
           int dist = 28607.8*pow(save, -0.9905);
           //12
           //float dist = 120225 * pow(save, -1.169);
           return dist;
}



