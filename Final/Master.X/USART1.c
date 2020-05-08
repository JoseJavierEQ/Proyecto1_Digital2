/*
 * File:   USART.c
 * Author: José Javier Estrada
 *
 * Created on February 10, 2020, 6:35 PM
 */


#include <xc.h>
#include "USART1.h"

void config(void){
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 25;
    SPBRGH = 0;
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    TXSTAbits.TXEN = 1;
}


void mandar(int dato){
    while (TXSTAbits.TRMT == 0){}
    TXREG = dato;
}