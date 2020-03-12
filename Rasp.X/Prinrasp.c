/*
 * File:   Prinrasp.c
 * Author: José Javier Estrada
 *
 * Created on March 12, 2020, 4:02 AM
 */

#include<pic16f887.h>               
#include <stdint.h>
#include<stdio.h>
#include "USART1.h"
#include "SPI.h"
#define _XTAL_FREQ 4000000

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF   

int valor1, valor2, valor3, valor4, contador = 0;
int lectura = 1;

void __interrupt() isr(void){
   if(SSPIF == 1){
        lectura = spiRead();
        if (lectura == 1){
           spiWrite(valor1); 
        }else if(lectura == 2){
            spiWrite(valor2); 
        }else if(lectura == 3){
            spiWrite(valor3); 
        }else{
            spiWrite(valor4); 
        }
        SSPIF = 0;
    }
}

void main(void) {
    ANSEL = 0B11111111;
    TRISA = 0B11111111;   
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    config();
    while(1)                        
    {
        if (RCIF){
            contador = RCREG;
            switch (contador){
                case 1:
                    while(!RCIF){
                    }
                    valor1 = RCREG;
                    break;
                case 2:
                    while(!RCIF){
                    }
                    valor2 = RCREG;
                    break;
                case 3:
                    while(!RCIF){
                    }
                    valor3 = RCREG;
                    break;
                case 4:
                    while(!RCIF){
                    }
                    valor4 = RCREG;
                    break;
                default:
                    while(!RCIF){
                    }
                    valor1 = RCREG;
                    break;
            }
        }    
    }
}
