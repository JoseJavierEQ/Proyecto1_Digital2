/* 
 * File:   LCD.h
 * Author: José Javier Estrada
 *
 * Created on February 10, 2020, 4:43 PM
 */

#ifndef __LCD_H
#define	__LCD_H
#define _XTAL_FREQ 4000000

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

#include <xc.h> 

#define lcd PORTB                   //----Assing PORTD as lcd
 
#define rs PORTDbits.RD5            //----Register Select Pin for Control of LCD
#define rw PORTDbits.RD6            //----Read/Write Pin for Control of LCD
#define en PORTDbits.RD7 

void lcd_cmd(unsigned char x);      //----Function to Send Command to LCD
void lcd_lat();                     //----Function to Latch data into LCD
void lcd_ready();                   //----Function to Check LCD is Busy
void LCD_DATO(int var);
void LCD_CURSOR(int a, int b);
void LCD_CHAR(char a);
void LCD_STRING(char *a);
void lcd_init(void);                    //----Initialization of LCD
 
void delay(unsigned int ms);   

#endif	/* LCD_H */

