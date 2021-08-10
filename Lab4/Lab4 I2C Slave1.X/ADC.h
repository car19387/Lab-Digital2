//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: ADC.h
 * Autor: Jefry Carrasco
 * Microcontrolador: PIC16F877
 * Creado: 6 de agosto, 2021
 * Modificado:  de agosto, 2021 */  

#ifndef ADC_H
#define	ADC_H

#include <xc.h> 

// Configuraci�n de canal, justificador e inicializaci�n
void ADCconfig(uint8_t canal, uint8_t just);

// Separaci�n de datos hexadecimales*/
void CONVhexa(uint8_t *valor, uint8_t *upper, uint8_t *lower);

#endif