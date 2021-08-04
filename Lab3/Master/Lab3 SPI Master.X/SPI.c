//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: SPI
 * Autor: Ligo George
 * Modificador: Jefry Carrasco
 * Company: electroSome
 * Project: SPI Library for MPLAB XC8
 * Microcontroller: PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Modificado:  de julio, 2021 */  

//******************************************************************************
// Librerías incluidas
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
#include "SPI.h"

//******************************************************************************
// Funciones
//******************************************************************************
void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    if(sType & 0b00000100) {        // Si es modo esclavo
        SSPSTAT = sTransmitEdge;    // Seleccionar flanco de transmisión
        TRISC3 = 1;                 // CLK como entrada
    }
    else                            // Si es modo maestro
    {
        SSPSTAT = sDataSample | sTransmitEdge; // Colocar el muestreo de datos 
                                               // y el flanco de transmisión
        TRISC3 = 0;                 //CLK como salida
    }
    SSPCON = sType | sClockIdle;
}

void sendSPI(char valor){
    SSPBUF = valor;                 // Coloca el valor en el SSPBUF
}

char readSPI(void){
    while(!SSPSTATbits.BF);         // Espera a que se finalice la transmisión
    return(SSPBUF);                 // Retornar el valor del SSPBUF
}
