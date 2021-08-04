//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: CSerial
 * Autor: Jefry Carrasco
 * Microcontrolador: PIC16F877
 * Creado: 19 de julio, 2021
 * Modificado:  de julio, 2021 */  

//******************************************************************************
// Librerías incluidas
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ADC.h"

//******************************************************************************
//  configuracion del ADC
//******************************************************************************
void ADCconfig(uint8_t canal, uint8_t just){
    ADCON1bits.ADFM = just;         // Justifiación 0 = izquierda y 1 = derecha
    ADCON1bits.VCFG0 = 0;           // Voltaje de referencia Vss y Vdd
    ADCON1bits.VCFG1 = 0; 
    
    switch(OSCCONbits.IRCF){
        case 0b100:
            ADCON0bits.ADCS =   0b000; // FOSC/0
            break;
        case 0b110:
            ADCON0bits.ADCS =   0b001; // FOSC/8
            break;
        case 0b111:
            ADCON0bits.ADCS =   0b010; // FOSC/32
            break;
        default:
            ADCON0bits.ADCS =   0b11;
            break;
    }
    
    ADCON0bits.CHS = canal;         // Seleccion de canal
    ADCON0bits.GO = 0;              // No iniciar conversion
    ADCON0bits.ADON = 1;            // Enecender módulo ADC
}

//******************************************************************************
//  Conversion de datos
//******************************************************************************
void CONVhexa(uint8_t *valor, uint8_t *upper, uint8_t *lower){
    
    uint8_t temp; //variable temporal para la separacion de datos
    temp = *valor;
    *lower = (*valor & 0x0F);//copia el nibble bajo
    temp = temp>>4;
    *upper = (temp & 0x0F);//copia el nibble alto
}
