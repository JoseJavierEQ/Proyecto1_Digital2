/*
 * File:   ADC.c
 * Author: José Javier Estrada
 *
 * Created on February 21, 2020, 12:52 PM
 */

#include <xc.h>
#define _XTAL_FREQ 4000000
#include "ADC.h"
#include <stdint.h>
#include<stdio.h>

void ADCinit(void) {
    ADCON0bits.ADCS0 = 1;
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADON = 1;
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;  
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    INTCONbits.GIE = 1;
    ADCON0bits.GO_DONE = 1;
    ADCON0bits.CHS0 = 0;      //CANAL AN0 PIN 1 DEL PIC
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;
}
