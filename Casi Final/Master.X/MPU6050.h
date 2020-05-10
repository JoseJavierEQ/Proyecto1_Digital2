/* 
 * File:   MPU6050.h
 * Author: José Javier Estrada
 *
 * Created on March 12, 2020, 5:16 AM
 */
//----------[ MPU6050 Register Map ]-----------

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __MPU6050_H
#define	__MPU6050_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

void MPU6050_Init(void);
int MPU6050_Read();

#endif	/* __I2C_H */
