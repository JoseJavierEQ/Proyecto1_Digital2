/*
 * File:   MPU6050.c
 * Author: José Javier Estrada
 *
 * Created on March 12, 2020, 5:15 AM
 */

#include <xc.h>
#include "I2C.h"
#include "MPU6050.h"
#include <stdio.h>

//-------------[ MPU6050 Routines ]------------------
//---------------------------------------------------

void MPU6050_Init()
{
 
  // Setting The Sample (Data) Rate
  I2C_Start(0xD0);
  I2C_Master_Write(SMPLRT_DIV);
  I2C_Master_Write(0x07);
  I2C_Master_Stop();
 
  // Setting The Clock Source
  I2C_Start(0xD0);
  I2C_Master_Write(PWR_MGMT_1);
  I2C_Master_Write(0x01);
  I2C_Master_Stop();
 
  // Configure The DLPF
  I2C_Start(0xD0);
  I2C_Master_Write(CONFIG);
  I2C_Master_Write(0x00);
  I2C_Master_Stop();
 
  // Configure The ACCEL (FSR= +-2g)
  I2C_Start(0xD0);
  I2C_Master_Write(ACCEL_CONFIG);
  I2C_Master_Write(0x00);
  I2C_Master_Stop();
 
  // Configure The GYRO (FSR= +-2000d/s)
  I2C_Start(0xD0);
  I2C_Master_Write(GYRO_CONFIG);
  I2C_Master_Write(0x18);
  I2C_Master_Stop();
 
  // Enable Data Ready Interrupts
  I2C_Start(0xD0);
  I2C_Master_Write(INT_ENABLE);
  I2C_Master_Write(0x01);
  I2C_Master_Stop();
}

void MPU6050_Read()
{
  int Ax,Ay,Az,T,Gx,Gy,Gz;
  float AX, AY, AZ, t, GX, GY, GZ;
  // Prepare For Reading, Starting From ACCEL_XOUT_H
  I2C_Start(0xD0);
  I2C_Master_Write(ACCEL_XOUT_H);
  I2C_Master_Stop();
  I2C_Start(0xD1);
  Ax = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Ay = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Az = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  T  = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gx = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gy = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gz = ((int)I2C_Read(0)<<8) | (int)I2C_Read(1);
  I2C_Master_Stop();

  // Convert The Readings
  AX = (float)Ax/16384.0;
  AY = (float)Ay/16384.0;
  AZ = (float)Az/16384.0;
  GX = (float)Gx/131.0;
  GY = (float)Gy/131.0;
  GZ = (float)Gz/131.0;
  t = ((float)T/340.00)+36.53;
}