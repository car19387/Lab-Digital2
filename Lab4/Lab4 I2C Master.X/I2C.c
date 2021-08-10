/* File:   I2C
 * Author: Jefry Alexander Carrasco Yac
 * Creado: 6 de agosto, 2021
 * Modificado:  de julio, 2021
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "I2C.h"

//******************************************************************************
// Funciones atribuibles al Master
//******************************************************************************
void MasterInit_I2C(unsigned long frec){
    SSPCON = 0x28; //Configurado en modo I2C maestro y habilitados pines
    SSPCON2 = 0X00; //inicializando bits de uso en transmision
    SSPSTAT = 0X80; //inicializando bits de control en transmision
    SSPADD = (_XTAL_FREQ/(4*frec))-1; //cargar en ADD el valor para la
                    //frecuencia que se espera
    TRISCbits.TRISC3 = 1; //entradas pero manipuladas acorde
    TRISCbits.TRISC4 = 1; //a lo que se le indique en la operacion
}

void waitCondition(void){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); 
    /*condicion de seguridad para esperar las operaciones con los datos
     enviados o recibidos*/
}

void MasterStart_I2C(void){
    waitCondition();
    SSPCON2bits.SEN = 1; //inicia la condicion de start
}

void MasterStop_I2C(void){
    waitCondition();
    SSPCON2bits.PEN = 1;//inicia la condicion de detenido
}

void MasterRepeatS_I2C(void){
    waitCondition();
    SSPCON2bits.RSEN = 1; //reinicia la comunicacion
}

void MasterSend_I2C(uint8_t dato){
    waitCondition();
    SSPBUF = dato; //coloca el dato en el buffer para enviarlo
}

void MasterReceive_I2C(uint8_t *valor){
    waitCondition();
    SSPCON2bits.RCEN = 1; //habilita la recepcion
    waitCondition(); //espera a que el dato se termine de colocar en el buffer
    *valor = SSPBUF; //copia el valor a la variable especificada
    waitCondition();
    SSPCON2bits.ACKDT = 1; //indica que se recibio el dato
    SSPCON2bits.ACKEN = 1; //comienza el acknowledge
    
}

//******************************************************************************
// Funciones atribuibles al Slave
//******************************************************************************
void SlaveInit_I2C(uint8_t address){
    SSPADD = address;       // Direccion que el slave va a utilizar
    SSPCON = 0X26;          // Slave con direccion de 7 bits y habilitados los pines
    SSPCON2 = 0X00;         // Pines de condiciones de transmision
    SSPSTAT = 0X80;         // Sin slew rate y condiciones iguales al master
    TRISCbits.TRISC3 = 1;   // Entradas pero manipuladas acorde
    TRISCbits.TRISC4 = 1;   // A lo que se le indique en la operacion
    /*Es necesario iniciar sus bits de interrupcion en el codigo principal*/
}
