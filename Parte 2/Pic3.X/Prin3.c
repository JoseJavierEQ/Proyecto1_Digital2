/*
 * File:   Prin3.c
 * Author: José Javier Estrada
 * Esclavo 3 el sensor de proximidad.
 * Created on February 29, 2020, 11:37 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma configx IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF   
 
#define Echo PORTAbits.RA0            
#define Trigger PORTAbits.RA1           
#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include "I2C.h"
#include<stdio.h>
#define  _XTAL_FREQ 4000000

//PROTOTIPO DE FUNCIÓN PARA MEDIR DISTANCIA
void mide_distancia(void);
uint16_t tiempo_eco=0;  //tiempo que dura el eco
int  distancia=0;  
uint8_t z;
uint8_t recibido;

void __interrupt() ISR(void){
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {//escribir
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            recibido = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){//leer
            z = SSPBUF;
            BF = 0;
            SSPBUF = distancia;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}
void main(void) {
    I2C_Slave_Init(0x60);
    //Configurar tmr1
    T1CONbits.T1CKPS = 0b00;
    T1CONbits.T1OSCEN = 1;
    T1CONbits.TMR1CS = 0;
    //configurar pin A1 entrada y pin A0 salida
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 0;
    PORTAbits.RA1 = 0;
    ANSEL = 0b00000000;
    while(1){                  //Ciclo infinito
      TMR1H = 0;
      TMR1L = 0;
      mide_distancia();          //Llamamos la función mide_distancia()
      __delay_ms(30);
   } 
}

void mide_distancia(void){       //Función mide_distancia
   PORTAbits.RA1 = 1;
   __delay_us(10);                 //Esperamos 10 us
   PORTAbits.RA1 = 0;
   while(!PORTAbits.RA0){
   }                             //espera flanco de subida
   TMR1ON = 1;                //Iniciamos el timer1 
   while(PORTAbits.RA0 && !TMR1IF){
   }                             //esperamos flanco de bajada
   TMR1ON = 0;
   if(!TMR1IF){
       tiempo_eco = TMR1H;
       tiempo_eco<<= 8;
       tiempo_eco |= TMR1L;
   }else{
       tiempo_eco = 0;
       TMR1IF = 0;
   }
   distancia = (tiempo_eco*0.5*34*0.001); //Convertimos a distancia a cm
   __delay_ms(200);                //Esperamos 50ms
}


  