//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: I2C
 * Autor: Jefry Carrasco
 * Microcontrolador: PIC16F877
 * Creado: 6 de agosto, 2021
 * Modificado:  de agosto, 2021 */  

//******************************************************************************
// Librerías incluidas
//******************************************************************************
#include "I2C.h"


// Función para inicializar I2C Maestro
void MasterInit_I2C(unsigned long frec){
    SSPCON = 0X28; //Configurado en modo I2C maestro y habilitados pines
    SSPCON2 = 0X00; //inicializando bits de uso en transmision
    SSPSTAT = 0X80; //inicializando bits de control en transmision
    SSPADD = (_XTAL_FREQ/(4*frec))-1; //cargar en ADD el valor para la
                    //frecuencia que se espera
    TRISCbits.TRISC3 = 1; //entradas pero manipuladas acorde
    TRISCbits.TRISC4 = 1; //a lo que se le indique en la operacion
}

// Función para inicializar I2C Esclavo
void SlaveInit_I2C(uint8_t address){
    SSPADD = address; //direccion que el slave va a utilizar
    SSPCON = 0X26; //slave con direccion de 7 bits y habilitados los pines
    SSPCON2 = 0X00; //pines de condiciones de transmision
    SSPSTAT = 0X80; // sin slew rate y condiciones iguales al master
    TRISCbits.TRISC3 = 1; //entradas pero manipuladas acorde
    TRISCbits.TRISC4 = 1; //a lo que se le indique en la operacion
    /*Es necesario iniciar sus bits de interrupcion en el codigo principal*/
}

// Función de espera si se está realizando una acción
void waitCondition(void){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); 
    /*condicion de seguridad para esperar las operaciones con los datos
     enviados o recibidos*/
}

// Función de inicio de la comunicación I2C
void MasterStart_I2C(void){
    waitCondition();
    SSPCON2bits.SEN = 1; //inicia la condicion de start
}

// Función de parada de la comunicación I2C
void MasterStop_I2C(void){
    waitCondition();
    SSPCON2bits.PEN = 1;//inicia la condicion de detenido
}

//Función de transmisión de datos del maestro al esclavo
void MasterSend_I2C(uint8_t dato){
    waitCondition();
    SSPBUF = dato; //coloca el dato en el buffer para enviarlo
}

//Función de recepción de datos enviados por el esclavo al maestro
void MasterReceive_I2C(uint8_t *valor){
    waitCondition();
    SSPCON2bits.RCEN = 1; //habilita la recepcion
    waitCondition(); //espera a que el dato se termine de colocar en el buffer
    *valor = SSPBUF; //copia el valor a la variable especificada
    waitCondition();
    SSPCON2bits.ACKDT = 0; //indica que se recibio el dato
    SSPCON2bits.ACKEN = 1; //comienza el acknowledge
}

