//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: CSerial
 * Autor: Jefry Carrasco
 * Microcontrolador: PIC16F877
 * Creado: 31 de julio, 2021
 * Modificado:  de julio, 2021 */  

//******************************************************************************
// Librerías incluidas
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
#include "CSerial.h"

//******************************************************************************
// Funciones
//******************************************************************************
void configUART(void){              // Configuración del EUSART
    TRISCbits.TRISC6 = 0;           // TX como salida
    TRISCbits.TRISC7 = 1;           // RX como entrada
    SPBRG = 12;                     // Valor para baud rate
    TXSTAbits.BRGH = 0;             // Low Speed
    TXSTAbits.TXEN = 1;             // Se habilita la transimisión
    RCSTAbits.CREN = 1;             // Se habilita recibimiento continuo
    TXSTAbits.SYNC = 0;             // Comunicación asincrona
    RCSTAbits.SPEN = 1;             // Se habilita TX y RX
}

void send1dato(char dato){          // Enviar daots 
    TXREG = dato;                   // Dato a enviar
    while(!TXSTAbits.TRMT);         // Esperar a que finalice la transmisión
}

void sendString(unsigned char* mensaje){ // Enviar mensaje
    while(*mensaje != 0x00){        // Si es distinto a 0
        send1dato(*mensaje);        // Enviar dato
        mensaje ++;                 // Siguiente letra
    }
}

void sendfloat(const float valor){
    uint8_t entero;
    uint8_t decimal;
    float temp;
    unsigned char digdecimal[1];
    entero = valor;
    temp = valor-(float)entero;
    decimal = (temp*100);
    division(decimal,&digdecimal[0],&digdecimal[1]);
    send1dato(entero+48);
    sendString(".");
    send1dato(digdecimal[1]+48);
    send1dato(digdecimal[0]+48);
    sendString("   ");
    
}

void sendhex(uint8_t *valor){
    uint8_t centena;
    uint8_t decena;
    uint8_t unidad;
    
    divisiondecimal(*valor,&unidad,&decena,&centena); // Separa los valores
    send1dato(centena +48);                           // COnversion ASCII
    send1dato(decena +48);
    send1dato(unidad +48);
}

void division(uint8_t conteo,uint8_t* un,uint8_t* dec){
    uint8_t div = conteo;
    *un =   0;
    *dec =  0;
    //modifica los valores de las variables asignadas de forma inmediata
    
    while (div >= 10){
    *dec = div/10;
    div = div - (*dec)*(10);
    }
    
    *un = div;
}

void divisiondecimal(uint8_t conteo,uint8_t* un,uint8_t* dec,uint8_t* cent){
    uint8_t div = conteo;
    *un =   0;
    *dec =  0;
    *cent = 0;
    //modifica los valores de las variables asignadas de forma inmediata
    
    while(div >= 100){
    *cent = div/100;
    div = div - (*cent)*(100);
    }
    
    while (div >= 10){
    *dec = div/10;
    div = div - (*dec)*(10);
    }
    
    *un = div;
}
