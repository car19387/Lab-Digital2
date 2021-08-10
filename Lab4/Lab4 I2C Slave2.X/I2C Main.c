//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: I2C Main
 * Dispositivo: PIC16f887
 * Autor: Jefry Carrasco
 * Hardware: 
 * 1 PIC16F887
 * 1 LCD 16*2
 * Sensor de temperatura
 * Resistencias de 200ohms
 * 2 Potenciometro
 * 2 Push button
 * Creado: 6 de agost, 2021
 * Modificado: de agosto, 2021 */  

//******************************************************************************
// Librerías incluidas
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <proc/pic16f887.h>
#include "I2C.h"

//******************************************************************************
// Configuración de PIC16f887
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
// Directivas del preprocesador
//******************************************************************************
#define _XTAL_FREQ 8000000      // Libreria para delay

//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void config(void);

//******************************************************************************
// Variables
//******************************************************************************
uint8_t clean;                  // Limpieza de bufer
uint8_t dato;                   // Envio de datos

//******************************************************************************
//  loop principal
//******************************************************************************
void main(void) {
    config();                   // Configuración general
    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){                   
        PORTD = dato;           // Se guarda dato en PORTD
    }
}

//******************************************************************************
//  Rutina de interrupcion
//******************************************************************************
void __interrupt() interrupcion(void){
    if(INTCONbits.RBIF){                // Interrupción de IOCB
        if(PORTBbits.RB0){              // Si se presiona RB0
            if(dato>=15)dato = 0;       // Si supera 15 lo regresa a 0
            else dato++;                // De lo contario aumenta
        }
        
        if(PORTBbits.RB1){              // Si se preciona RB1
            if(dato<=0)dato = 15;       // Si es menor a 0 lo regresa a 15
            else dato--;                // De lo contrario resta
        }
        INTCONbits.RBIF = 0;            // Se apaga la bandera
    }
    
    if(PIR1bits.SSPIF){                 // Interrupción de MSSP
        SSPCONbits.CKP = 0;             // Clock low
        
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            clean = SSPBUF;             // Guardar dato del buffer
            SSPCONbits.SSPOV = 0;       // No overflow
            SSPCONbits.WCOL = 0;        // No colisión
            SSPCONbits.CKP = 1;         // Habilitar clock
        }
        
        if(!SSPSTATbits.R && !SSPSTATbits.D){
            clean = SSPBUF;             // Limpiar el buffer
            SSPCONbits.CKP = 1;         // Habilitar clock
            while(!SSPSTATbits.BF);     // Espera a que se llene el buffer
            PORTB = SSPBUF;             // Guarda el balor del buffer en PORTB
            __delay_ms(1);
        }
        
        else if(SSPSTATbits.R && !SSPSTATbits.D){
            clean = SSPBUF;             // Limpiar el buffer
            SSPSTATbits.BF = 0;         // Recibimiento no completo
            SSPBUF = dato;              // Coloca el dato en el buffer
            SSPCONbits.CKP = 1;         // Habilita clock
            __delay_ms(1);
            while(SSPSTATbits.BF);      // Espera el envio
        }
        PIR1bits.SSPIF = 0;             // Se apaga la bandera
    }   
}

//******************************************************************************
// Configuracion
//******************************************************************************
void config(void){
    // Configuración de Puertos
    ANSEL = 0x00;               // Pines digitales
    ANSELH = 0x00;
    
    TRISA = 0x00;               // PORTA como salida
    TRISB = 0x03;               // PORTB0 y PORB1 como entrada
    TRISD = 0x00;               // PORTD como salida
    
    PORTA = 0x00;               // Limpiar puertos
    PORTB = 0x00;
    PORTD = 0x00;
    
    IOCBbits.IOCB0 = 1;         // RB0 y RB1 con IOCB
    IOCBbits.IOCB1 = 1;
    
    // Configuracion del oscilador
    OSCCONbits.IRCF = 0b111;    // Frecuencia a 8MHZ
    OSCCONbits.SCS = 1;         // Habilitar reloj interno
    
    // Configuración de interrupciones
    INTCONbits.GIE = 1;         // Se habilitan las interrupciones globales
    INTCONbits.PEIE = 1;        // Se habilitan las interrupciones perifericas
    PIE1bits.SSPIE = 1;         // Se habilitan las interrupciones del I2C
    PIR1bits.SSPIF = 0;         // Se apaga la bandera de I2C
    INTCONbits.RBIF = 0;        // Se habilitan las IOCB
    INTCONbits.RBIE = 1;        // Se apaga la bandera del IOCB
    SlaveInit_I2C(0X30);        // Se inicializa I2C
}
