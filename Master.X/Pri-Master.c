/*
 * File:   Pri-Master.c
 * Author: José Javier Estrada
 *
 * Created on February 21, 2020, 12:44 PM
 */

#include<pic16f887.h>              
#include "LCD.h" 
#include <stdint.h>
#include<stdio.h>
#include "USART1.h"
#include "MPU6050.h"
#include "I2C.h"
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

#define lcd PORTB                   //----Assing PORTD as lcd
 
#define rs PORTDbits.RD5            //----Register Select Pin for Control of LCD
#define rw PORTDbits.RD6            //----Read/Write Pin for Control of LCD
#define en PORTDbits.RD7           //----Enable Pin for Control of LCD

int valor1, valor2, valor3, valor4, mot1, mot2, mot3, ala = 0;
char valor[16];

void main(void) {
    TRISD = 0x00;                    //----Set as output
    TRISB = 0x00;                    //----Set as output
    I2C_Master_Init(100000); 
    MPU6050_Init();
    config();
    lcd_init();                     //----Initialize LCd
    LCD_STRING("JOSE JAVIER");        //----SEnd data of String
    LCD_DATO(0xC0);                  //----Next line 1st location
    LCD_STRING("ESTRADA  #17078");     //----Send data
    delay(500);                    //----1 sec
    LCD_DATO(0x01);                  //----LCD Clear
    LCD_CURSOR(1,1);
    LCD_STRING(" V1    V2     V3");
    while(1)                        //-----Creating Super Loop
    {
        //Recibir los valores I2C
        I2C_Master_Start();
        I2C_Master_Write(0x51);     // Fotoresistencia
        valor1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0x61);     // Ultrasonico
        valor2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);

         
        I2C_Master_Start();
        I2C_Master_Write(0x71);     // Lluvia
        valor3 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        
        I2C_Master_Start();
        I2C_Master_Write(0x81);     // Seguidor de linea
        valor4 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        // FALTA GIROSCOPIO
        
        //Condiciones motores
        if (valor1 == 1){
            mot1 = 1;
        }else{
            mot1 = 0;
        }

        if (valor2 <=10){
          ala = 1;
        }else{
          ala = 0;
        }
        if (valor4 == 1 && ala != 1){
            mot2 = 1;
        }else{
            mot2 = 0;
        }
        
        if (valor3 >= 100){
            mot3 = 1;
        }else{
            mot3 = 0;
        }
        
        // Enviar datos SPI motores
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(mot1);     // Encender rutina de servo
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0x60);     // Encender DC
        I2C_Master_Write(mot2);
        I2C_Master_Stop();
        __delay_ms(10);

        I2C_Master_Start();
        I2C_Master_Write(0x70);     // Encender Stepper
        I2C_Master_Write(mot3);
        I2C_Master_Stop();
        __delay_ms(10);        
        
        // Enviar valores USART
        mandar(1);
        mandar(valor1);
        mandar(2);
        mandar(valor2);
        mandar(3);
        mandar(valor3);
        mandar(4);
        mandar(valor1);
        mandar(5);
        mandar(valor2);
  
    }
}
