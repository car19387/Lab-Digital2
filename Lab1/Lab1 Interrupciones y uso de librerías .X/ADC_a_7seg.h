/* File: ADC a 7seg
 * Author: Jefry Alexander Carrasco
 * Creado: 19 de julio, 2021
 * Modificado:  de julio, 2021
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
void hexconv(uint8_t val, uint8_t *decena, uint8_t *unidad);
void ADCconfig(uint8_t canal, uint8_t just);
uint8_t display(uint8_t val);
#endif	/* XC_HEADER_TEMPLATE_H */

