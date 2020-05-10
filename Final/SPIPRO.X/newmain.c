//*****************************************************************************
/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de implementación de la comunicación SPI 
 * Código Maestro
 * Created on 10 de febrero de 2020, 03:32 PM
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
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
#pragma config WRT = OFF          // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include<pic16f887.h> 
#include <stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include "SPI.h"
#include "LCD.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
int a, b, c, d, e, A, B, C, D, E, A1, B1, C1, D1, E1 = 0;
char valor[16];
char valor1[16];


//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    lcd_init();                     //----Initialize LCd
    LCD_STRING("JOSE JAVIER");        //----SEnd data of String
    LCD_DATO(0xC0);                  //----Next line 1st location
    LCD_STRING("ESTRADA  #17078");     //----Send data
    __delay_ms(500);                   //----1 sec
    LCD_DATO(0x01);                  //----LCD Clear
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1) {
        sprintf(valor1, "F Ll Dis Li  Sec");
        LCD_CURSOR(1, 1); //-----Last lacation of 1st Line
        LCD_STRING(valor1);
        sprintf(valor, "%.1d %.1d  %.3d %.1d    %.d", C1, E1, D1, A1, B1);
        LCD_CURSOR(2, 1); //-----Last lacation of 1st Line
        LCD_STRING(valor);
        __delay_ms(1);
        spiWrite(1);
        A1 = spiRead();
        __delay_ms(10);
        spiWrite(2);
        B1 = spiRead();
        __delay_ms(10);
        spiWrite(3);
        C1 = spiRead();
        __delay_ms(10);
        spiWrite(4);
        D1 = spiRead();
        __delay_ms(10);
        spiWrite(5);
        E1 = spiRead();
        __delay_ms(10);
//        spiWrite(7);
//        b = spiRead();
//        __delay_ms(10);
//        spiWrite(8);
//        c = spiRead();
//        __delay_ms(10);
//        spiWrite(9);
//        d = spiRead();
//        __delay_ms(10);
//        spiWrite(10);
//        e = spiRead();
//        __delay_ms(10);
    }
//    B1 = B + b;
//    C1 = C + c;
//    D1 = D + d;
//    E1 = E + e;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISC2 = 0;
    TRISD = 0x00;                    //----Set as output
    TRISB = 0x00;                    //----Set as output
    TRISCbits.TRISC0 = 1;
    ANSEL = 0;
    ANSELH = 0;
    PORTB = 0;
    PORTA = 0;
    PORTD = 0;
    ANSEL = 0B11111111;
    TRISA = 0B11111111; 
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    PORTCbits.RC2 = 0;       //Slave Select
}