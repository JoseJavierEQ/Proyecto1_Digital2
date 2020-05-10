/* 
 * File:   USART.h
 * Author: José Javier Estrada
 *
 * Created on February 10, 2020, 6:32 PM
 */

#ifndef __USART_H
#define	__USART_H

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

#include <xc.h> 

void configu(void);
void mandar(int dato);

#endif	/* USART_H */

