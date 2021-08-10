//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: I2C.h
 * Autor: Jefry Carrasco
 * Microcontrolador: PIC16F877
 * Creado: 6 de agosto, 2021
 * Modificado:  de agosto, 2021 */ 

#ifndef I2C_H
#define	I2C_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#include <xc.h> 
#include <stdint.h>
#include <pic16f887.h>

// Funci�n para inicializar I2C Maestro
void MasterInit_I2C(unsigned long frec);

// Funci�n para inicializar I2C Esclavo
void SlaveInit_I2C(uint8_t address);

// Funci�n de espera si se est� realizando una acci�n
void waitCondition(void);

// Funci�n de inicio de la comunicaci�n I2C
void MasterStart_I2C(void);

// Funci�n de parada de la comunicaci�n I2C
void MasterStop_I2C(void);

//Funci�n de transmisi�n de datos del maestro al esclavo
void MasterSend_I2C(uint8_t dato);

//Funci�n de recepci�n de datos enviados por el esclavo al maestro
void MasterReceive_I2C(uint8_t *valor);

#endif