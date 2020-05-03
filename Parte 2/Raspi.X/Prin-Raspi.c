/*
 * File:   Prin-Raspi.c
 * Author: José Javier Estrada
 *
 * Created on March 15, 2020, 1:31 PM
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

int a, b, c, d, e, A, B, C, D, E, A1, B1, C1, D1, E1 = 0;
int lectura = 0;

void __interrupt() isr(void){
   if(SSPIF == 1){
        lectura = spiRead();
        if (lectura == 1){
            spiWrite(A*2);
        }else if(lectura == 2){
            spiWrite(B*2);
        }else if(lectura == 3){
            spiWrite(C*2);
        }else if(lectura == 4){
            spiWrite(D*2);
        }else if (lectura== 5){
            spiWrite(E*2); 
        }else if (lectura== 6){
            spiWrite(a*2); 
        }else if (lectura== 7){
            spiWrite(b*2); 
        }else if (lectura== 8){
            spiWrite(c*2); 
        }else if (lectura== 9){
            spiWrite(d*2); 
        }else if (lectura== 10){
            spiWrite(e*2); 
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
    while (1){\
        // Valor 1 Fotoresistencia
        mandar(1);
        while(!RCIF){
        }
        A1 = RCREG;
        if(A1%2){
            A = A1/2;
            a = A+1;
        } else{
            A = A1/2;
            a = A;
        }
        __delay_ms(100);

        // Valor 2 Ultrasonico
        mandar(2);
        while(!RCIF){
        }
        B1 = RCREG;
        if(B1%2){
            B = B1/2;
            b = B+1;
        } else{
            B = B1/2;
            b = B;
        }
        __delay_ms(100);
        
        // Valor 3 Lluvia
        mandar(3);
        while(!RCIF){
        }
        C1 = RCREG;
        if(C1%2){
            C = C1/2;
            c = C+1;
        } else{
            C = C1/2;
            c = C;
        }
        __delay_ms(100);
//        
//        // Valor 4 Seguidor de linea
        mandar(4);
        while(!RCIF){
        }
        D1 = RCREG;
        if(D1%2){
            D = D1/2;
            d = D+1;
        } else{
            D = D1/2;
            d = D;
        }
        __delay_ms(100);
//        
//        // Valor 5 Tiempo
        mandar(5);
        while(!RCIF){
        }
        E1 = RCREG;
        if(E1%2){
            E = E1/2;
            e = E+1;
        } else{
            E = E1/2;
            e = E;
        }
        __delay_ms(100);
    }
}
