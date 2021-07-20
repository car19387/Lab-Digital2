/* File: ADC a 7seg
 * Author: Jefry Alexander Carrasco
 * Creado: 19 de julio, 2021
 * Modificado:  de julio, 2021
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ADC_a_7seg.h"

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
//  Conversion bin a hex
//******************************************************************************
void hexconv(uint8_t val, uint8_t *decena, uint8_t *unidad){
    
    uint8_t temp; 
    temp = val;
    *unidad = (val & 0x0F);             // Copiar el nibble bajo
    temp = temp>>4;                     // Rotacion de byte
    *decena = (temp & 0x0F);            // Copia el nibble alto
}

//******************************************************************************
//  Display 7seg
//******************************************************************************
uint8_t display(uint8_t val){

    switch(val){                        // Tabla para display 7seg
        case 0:
        return 0b00111111;	break;      //0
        case 1:
        return 0b00000110; break;       //1
        case 2:
        return 0b01011011;	break;      //2
        case 3:
        return 0b01001111;	break;      //3
        case 4:
        return 0b01100110;	break;      //4
        case 5:
        return 0b01101101;	break;      //5
        case 6:
        return 0b01111101;	break;      //6
        case 7:
        return 0b00000111;	break;      //7
        case 8:
        return 0b01111111;	break;      //8
        case 9:
        return 0b01100111;	break;      //9   
        case 10:
        return 0b01110111;	break;      //A   
        case 11:
        return 0b01111100;	break;      //B   
        case 12:
        return 0b00111001;	break;      //C   
        case 13:
        return 0b01011110;	break;      //D   
        case 14:
        return 0b01111001;	break;      //E   
        case 15:
        return 0b01110001;	break;      //F   
        default:
        return 0b00111111;	break;      //0
    }
}