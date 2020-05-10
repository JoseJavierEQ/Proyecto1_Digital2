/*
 * File            : I2C.c
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordoñez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 */
#include "I2C.h"
#include <stdint.h>
#define I2C_BaudRate           100000
#define SCL_D                  TRISC3
#define SDA_D                  TRISC4

//*****************************************************************************
// Función para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init(const unsigned long c)
{
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}
//*****************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//*****************************************************************************
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
//*****************************************************************************
// Función de inicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Start()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.SEN = 1;    //inicia la comunicación i2c
}
//*****************************************************************************
// Función de reinicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RSEN = 1;   //reinicia la comunicación i2c
}
//*****************************************************************************
// Función de parada de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Stop()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.PEN = 1;    //detener la comunicación i2c
}
//*****************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d)
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPBUF = d;             
}
//*****************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//*****************************************************************************
unsigned short I2C_Master_Read(unsigned short a)
{
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato leído
}
//*****************************************************************************
// Función para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address)
{ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}
//*****************************************************************************
void I2C_Start(char add)
{
    I2C_Master_Wait();
    SEN = 1;
    I2C_Master_Write(add);
}

void I2C_ACK(void)
{
	ACKDT = 0;			// 0 -> ACK
    ACKEN = 1;			// Send ACK
    while(ACKEN);
}

void I2C_NACK(void)
{
	ACKDT = 1;			// 1 -> NACK
	ACKEN = 1;			// Send NACK
    while(ACKEN);
}
void I2C_Master_Init1()
{
  SSPCON  = 0x28;
  SSPCON2 = 0x00;
  SSPSTAT = 0x00;
  SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;
  SCL_D = 1;
  SDA_D = 1; 
}
unsigned char I2C_Read(unsigned char ACK_NACK)
{  
    //---[ Receive & Return A Byte & Send ACK or NACK ]---
    unsigned char Data;
    RCEN = 1;              
    while(!BF);      
    Data = SSPBUF;           
    if(ACK_NACK==0)
        I2C_ACK();            
    else
        I2C_NACK();     
    while(!SSPIF);                 
    SSPIF=0;   
    return Data;
}

void i2c_START(){
    if(SSPSTATbits.S == 1){
        SSPSTATbits.S = 0;
    }
    PIR1bits.SSPIF = 0;
    SSPCON2bits.SEN = 1;
    while(SSPSTATbits.S == 0);
    while(PIR1bits.SSPIF == 0);
    return;
}

void i2c_WRITE(unsigned int DATA){
    while(SSPCON2bits.SEN);//end of condition start
    SSPCON2bits.ACKSTAT = 1; // none Ack receive, of corse
    PIR1bits.SSPIF = 0;
    SSPSTATbits.BF = 0;
    SSPBUF = DATA;
    while(PIR1bits.SSPIF == 0 && SSPSTATbits.BF == 0); //trasmit begin
    PIR1bits.SSPIF = 0;
    while(SSPSTATbits.BF == 1); //end of transmition 8ºbit(cicly of clock)
    return;
}

void i2c_ACK_Rec(){
    while(SSPCON2bits.ACKSTAT == 1 && PIR1bits.SSPIF == 0); // wait ACK signal
    PIR1bits.SSPIF = 0;
    SSPCON2bits.ACKSTAT = 1;
    return ;
}

unsigned int i2c_READ(){
    unsigned int r_buff;
    PIR1bits.SSPIF = 0;
    SSPSTATbits.BF = 0;
    SSPCON2bits.RCEN = 1; // enable Receive mode, follow my rotins the bus must to be idle
    while(SSPCON2bits.RCEN == 1);
    while(SSPSTATbits.BF == 0 && PIR1bits.SSPIF == 0); // wait SSPBUF receive SSPSR data
    //while(SSPSTATbits.BF == 1); // buff is read
    r_buff = SSPBUF;
    PIR1bits.SSPIF = 0;
    SSPSTATbits.BF = 0;
    return r_buff;
}

void i2c_ACK_Send(){
    PIR1bits.SSPIF = 0;
    SSPCON2bits.ACKDT = 0;
    SSPCON2bits.ACKEN = 1;
    while(PIR1bits.SSPIF == 0);
    return;
}

void i2c_NACK_Send(){
    PIR1bits.SSPIF = 0;
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
    while(PIR1bits.SSPIF == 0 && SSPCON2bits.ACKEN == 1);
    return;
}

unsigned char I2C_Master_Write1(unsigned char data)
{
    I2C_Master_Wait();
    SSPBUF = data;
    while(!SSPIF);  // Wait Until Completion
	SSPIF = 0;
    return ACKSTAT;
}

void i2c_STOP(){
    int cont = 0;
    SSPSTATbits.P = 0;
    PIR1bits.SSPIF = 0;
    SSPCON2bits.PEN = 1;
    while(SSPSTATbits.P == 0 && SSPCON2bits.PEN == 1 && PIR1bits.SSPIF == 0){ //wait P stop condition be detected
        /*cont++;
        if(cont>9999){
            SSPSTATbits.P = 1;
            SSPCON2bits.PEN = 0;
            PIR1bits.SSPIF = 1;
        }
        */
    }
    PIR1bits.SSPIF = 0;
    return;
}
void I2C_Write(unsigned data)
{
  I2C_Hold(); //Hold the program is I2C is busy 
  SSPBUF = data;         //pg82/234
}
void I2C_Hold()
{
    while (   (SSPCON2 & 0b00011111)    ||    (SSPSTAT & 0b00000100)   ) ; //check the bis on registers to make sure the IIC is not in progress
}

void I2C_Begin()
{
  I2C_Hold();  //Hold the program is I2C is busy  
  SEN = 1;     //Begin IIC pg85/234
}

void I2C_End()
{
  I2C_Hold(); //Hold the program is I2C is busy  
  PEN = 1;    //End IIC pg85/234
}

int  BCD_2_DEC(int to_convert)
{
   return (to_convert >> 4) * 10 + (to_convert & 0x0F); 
}

int DEC_2_BCD (int to_convert)
{
   return ((to_convert / 10) << 4) + (to_convert % 10);
}
