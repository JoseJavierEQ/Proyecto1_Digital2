/* 
 * File:   ADC.h
 * Author: José Javier Estrada
 *
 * Created on February 21, 2020, 12:51 PM
 */

#ifndef __ADC_H
#define _XTAL_FREQ 4000000

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

#include <xc.h> 

void ADCinit(void);

#endif	/* ADC_H */