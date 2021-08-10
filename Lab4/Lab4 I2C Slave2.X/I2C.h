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

// Función para inicializar I2C Maestro
void MasterInit_I2C(unsigned long frec);

// Función para inicializar I2C Esclavo
void SlaveInit_I2C(uint8_t address);

// Función de espera si se está realizando una acción
void waitCondition(void);

// Función de inicio de la comunicación I2C
void MasterStart_I2C(void);

// Función de parada de la comunicación I2C
void MasterStop_I2C(void);

//Función de transmisión de datos del maestro al esclavo
void MasterSend_I2C(uint8_t dato);

//Función de recepción de datos enviados por el esclavo al maestro
void MasterReceive_I2C(uint8_t *valor);

#endif