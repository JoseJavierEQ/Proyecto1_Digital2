/*
 * File:   Prin3.c
 * Author: José Javier Estrada
 *
 * Created on February 29, 2020, 11:37 PM
 */

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
 
#define Echo PORTAbits.RA0            
#define Trigger PORTAbits.RA1           
#include <xc.h>
#define  _XTAL_FREQ 4000000

//PROTOTIPO DE FUNCIÓN PARA MEDIR DISTANCIA
void mide_distancia(void);
int tiempo_eco=0;  //tiempo que dura el eco
int  distancia=0;  

void main(void) {
    //Configurar tmr0
    //configurar pin A1 entrada y pin A0 salida
    while(1){                  //Ciclo infinito
      mide_distancia();          //Llamamos la función mide_distancia()
      __delay_ms(30);              //Esperamos 30ms
   } 
}

void mide_distancia(void){       //Función mide_distancia
    Trigger = 1;
   __delay_us(10);                 //Esperamos 10 us
   Trigger = 0;
   while(!Echo){
   }                             //espera flanco de subida
   //set_timer1(0);                //Iniciamos el timer0 en 0 con libreria
   while(Echo){
   }                             //esperamos flanco de bajada
   //tiempo_eco = get_timer1();    //Leemos el valor del timer0 
   distancia = (tiempo_eco/2)*0.004*34; //Convertimos a distancia
   __delay_ms(200);                //Esperamos 200ms
}


  