

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  

void ADCconfig(uint8_t canal, uint8_t just);
/*configura el ADC y lee el valor del reloj para no violar los tiempos
 permite elegir el canal inicial y la justificacion, el cambio de 
 canal se puede hacer manual luego*/
void CONVhexa(uint8_t *valor, uint8_t *upper, uint8_t *lower);
/*convierte valores hexadecimal en sus dos nibbles, el alto y bajo*/

#endif	/* XC_HEADER_TEMPLATE_H */

