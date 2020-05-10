/*
 * File:   LCD.c
 * Author: José Javier Estrada
 *
 * Created on February 10, 2020, 4:28 PM
 */

#include "LCD.h"
#include <xc.h>
#define _XTAL_FREQ 4000000

void lcd_lat()                    //----To Latch data into LCD
{
    en = 1;                       //----Enable Pin goes high
    delay(1);                     //----delay of 1ms
    en = 0;                       //----Enable Pin goes Low
}
 
void lcd_ready()
{
    lcd = 0xFF;                   //----PORTD is High
    lcd &= 0x80;                  //----D7 is set as high
    rs = 0;                       //----Command Register is Selected
    rw = 1;                       //----Read/Write Pin is High => Read
    en = 0; delay(1); en = 1;     //----Low to High to read data from LCD
    if(lcd == 0x80)
    {
        en = 0; delay(1); en = 1;     //----Low to High to read data from LCD
    }
    else
    {
        //---Do nothing.
    }
}
 
void lcd_cmd(unsigned char x)
{
    lcd_ready();                  //----To Check whether lcd is busy
    lcd = x;                      //----8-Bit Command is Send to PORTD
    rs = 0;                       //----Register Select Pin is Low => Command Register
    rw = 0;                       //----Read/Write Pin is Low => Write.
    lcd_lat();                    //----Latch data into LCD
}
 
void lcd_init(void)
{
    lcd_cmd(0x38);              //----8-bit data and 16x2 line
    lcd_cmd(0x0E);              //----Cusor Blinking
    lcd_cmd(0x01);              //----Clear LCD
    lcd_cmd(0x06);              //----Increment Cusor
    lcd_cmd(0x80);              //----1st low of 1st row
}
 
void delay(unsigned int ms)
{
    unsigned int i,j;
    for(i=0;i<=120;i++)         //---To generate 1ms delay
        for(j=0;j<=ms;j++);     //---To generate user define delay
}

void LCD_DATO(int var){
    rs = 0;
    __delay_ms(5);
    en = 1;
    lcd = var;
    __delay_ms(5);
    en = 0;
    __delay_ms(5);
} 

void LCD_CURSOR(int a, int b)
{
    int d;
	if(a == 1)
	{
        d = 0x80 + b - 1;
		LCD_DATO(d);
	}
	else if(a == 2)
	{
		d = 0xC0 + b - 1;
		LCD_DATO(d);
	}
}



void LCD_CHAR(char a)
{   
    rs = 1;
    // LCD_Port(caracter);
    lcd = a;
    en = 1;
    __delay_us(40);
    en = 0;
}

void LCD_STRING(char *a)
{
    int n;
    for(n = 0; a[n] != '\0'; n++){
        LCD_CHAR(a[n]);
    }
}