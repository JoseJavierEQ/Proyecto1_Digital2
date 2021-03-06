/*
 * File:   Pri-Master.c
 * Author: Jos� Javier Estrada
 *
 * Created on February 21, 2020, 12:44 PM
 */

#include<pic16f887.h>              
#include "LCD.h" 
#include <stdint.h>
#include<stdio.h>
#include<stdlib.h>
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
#define A PORTAbits.RA1
#define B PORTAbits.RA3
#define C PORTAbits.RA4
#define D PORTAbits.RA5
#define rs PORTDbits.RD5            //----Register Select Pin for Control of LCD
#define rw PORTDbits.RD6            //----Read/Write Pin for Control of LCD
#define en PORTDbits.RD7           //----Enable Pin for Control of LCD
void PasoCompSimple(void);
void servo0(void);
void servo180(void) ;
unsigned short read_ds1307(unsigned short address){
unsigned short r_data;
  I2C_Master_Start();
  I2C_Master_Write(0xD0); //address 0x68 followed by direction bit (0 for write, 1 for read) 0x68 followed by 0 --> 0xD0
  I2C_Master_Write(address);
  I2C_Master_RepeatedStart();
  I2C_Master_Write(0xD1); //0x68 followed by 1 --> 0xD1
  r_data=I2C_Master_Read(0);
  I2C_Master_Stop();
  return(r_data);
}
unsigned char BCD2UpperCh(unsigned char bcd)
{
  return ((bcd >> 4) + '0');
}

unsigned char BCD2LowerCh(unsigned char bcd)
{
  return ((bcd & 0x0F) + '0');
}
int deter, valor1, valor2, valor3, valor4, valor5, mot1, mot2, mot3, ala, var = 0;
char valor[16];
char valorf[];
int second;
int minute;
int hour;
int hr;
int day;
int dday;
int month;
int year;
int ap;
char arriba[] = "S1:1 S2:1 d:000";
char abajo[] = "S4:1 00:00:00 AM";
char time[] = "00:00:00 PM";
char date[] = "00-00-00";

void main(void) {
    TRISD = 0x00;                    //----Set as output
    TRISB = 0x00;                    //----Set as output
    TRISA = 0X00;
    TRISCbits.TRISC0 = 1;
    ANSEL = 0;
    ANSELH = 0;
    PORTB = 0;
    PORTA = 0;
    PORTD = 0;
    PORTAbits.RA2 = 1;
    I2C_Master_Init(100000); 
    config();
    lcd_init();                     //----Initialize LCd
    LCD_STRING("JOSE JAVIER");        //----SEnd data of String
    LCD_DATO(0xC0);                  //----Next line 1st location
    LCD_STRING("ESTRADA  #17078");     //----Send data
    delay(500);                    //----1 sec
    LCD_DATO(0x01);                  //----LCD Clear
    while(1)                        //-----Creating Super Loop
    {
        //Recibir los valores I2C        
        I2C_Master_Start();
        I2C_Master_Write(0x51);     // Fotoresistencia
        valor1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(20);
        
        I2C_Master_Start();
        I2C_Master_Write(0x61);     // Ultrasonico
        valor2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(20);

         
        I2C_Master_Start();
        I2C_Master_Write(0x71);     // Lluvia
        valor3 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(20);
        
        // Reloj a tiempo real
        second = read_ds1307(0);
        minute = read_ds1307(1);
        hour = read_ds1307(2); 
        hr = hour & 0b00011111; 
        ap = hour & 0b00100000;
        day = read_ds1307(3);
        day = read_ds1307(4);
        month = read_ds1307(5);
        year = read_ds1307(6);
        time[0] = BCD2UpperCh(hr);
        time[1] = BCD2LowerCh(hr);
        time[3] = BCD2UpperCh(minute);
        time[4] = BCD2LowerCh(minute);
        time[6] = BCD2UpperCh(second);
        time[7] = BCD2LowerCh(second);
        date[0] = BCD2UpperCh(day);
        date[1] = BCD2LowerCh(day);
        date[3] = BCD2UpperCh(month);
        date[4] = BCD2LowerCh(month);
        date[6] = BCD2UpperCh(year);
        date[7] = BCD2LowerCh(year);
        valorf[1] = BCD2LowerCh(second);
        valorf[0] = BCD2UpperCh(second);
        valor5 = atoi(valorf); 
        if(ap)
        {
          time[9] = 'P';
          time[10] = 'M';
        }
        else
        {
          time[9] = 'A';
          time[10] = 'M';
        }  
        sprintf(valor, "%.1d %.1d %.3d %.1d %.2d", valor1, valor3, valor2, valor4, valor5);
        LCD_CURSOR(2,1);              //-----Last lacation of 1st Line
        LCD_STRING(valor); 
        LCD_CURSOR(1,1);
        LCD_STRING(time);

        // Enviar valores USART
        if (RCIF){
            deter = RCREG;
        }
        switch(deter){
        case 1:                             // 1
            mandar(valor1);
            __delay_ms(10);
            break;
        case 2:                             // 2
            mandar(valor2);
            __delay_ms(10);
            break;
        case 3:                             // 3
            mandar(valor3);
            __delay_ms(10);
            break;
        case 4:                             // 4
            mandar(valor4);
            __delay_ms(10);
            break;
        case 5:                             // 5
            mandar(valor5);
            __delay_ms(10);
            break;
        default:                            // 0
            mandar(valor5);
            __delay_ms(10);
            break; 
    }
    
//        Condiciones motores
//        Servo controlado por fotoresistencia
        if (valor1 == 1){          
            servo0(); //0�
            servo180(); //180�
        }else{
            PORTAbits.RA0 = 0;
        }
    
        // Motores DC
        if (PORTCbits.RC0==1){
            var = 1;
        }else {
            var = 0;
        } 
        if (PORTCbits.RC0==0){
            valor4 = 0;
        }else{
            valor4 = 1;
        }
        if (valor2 <=10 || PORTCbits.RC0==0){
           var = 0;
        }else{
            var = 1;
        }
        if (var ==1){
            PORTAbits.RA2 = 1;
        }else{
            PORTAbits.RA2 = 0;
        }


        // Motor Stepper
        if (valor3 == 1){
            for (int cont = 0; cont < 256; cont++){
                PasoCompSimple(); 
            }   
        }
    }
}

// Funciones Motores

void servo0(void) //0�
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    PORTAbits.RA0 = 1;
    __delay_us(1000);
    PORTAbits.RA0 = 0;
    __delay_us(19000);
  }
}
void servo180(void) //180�
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    PORTAbits.RA0 = 1;
    __delay_us(2000);
    PORTAbits.RA0 = 0;
    __delay_us(18000);
  }
}

void PasoCompSimple(void){
    A = 1;
    B = 0;
    C = 0;
    D = 0;
    __delay_ms(15);  //paso 1
    
    A = 0;
    B = 1;
    C = 0;
    D = 0;
    __delay_ms(15);  // paso 2
    
    A = 0;
    B = 0;
    C = 1;
    D = 0;
    __delay_ms(15);  // paso 3
    
    A = 0;
    B = 0;
    C = 0;
    D = 1;
    __delay_ms(15);  //paso 4
}
