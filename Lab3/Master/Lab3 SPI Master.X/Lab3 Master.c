//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: Lab3
 * Dispositivo: PIC16f887
 * Autor: Jefry Carrasco
 * Hardware: 
 * 2 PIC16F887
 * 2 Potenciometro
 * 1 Terminal Virtual
 * Creado: 30 de julio, 2021
 * Modificado:  de julio, 2021 */  

//******************************************************************************
// Librerías incluidas
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "SPI.h"
#include "CSerial.h"

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
//  Variables
//******************************************************************************
uint8_t val1,val2,UARTdat,UARTsend,UARTsend2,contador;
float potf1,potf2;
//******************************************************************************
//  loop principal
//******************************************************************************
void main(void) {
    config();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){
        PORTCbits.RC2 = 0;
        sendSPI('1');           // Enviar dato para recepcion
        UARTsend = readSPI();   // Guardar lectura
        __delay_ms(10);         // Delay de 10ms
        
        sendSPI('2');           // Enviar dato para recepcion
        UARTsend2 = readSPI();  // Guardar lectura2
        __delay_ms(10);         // Delay de 10ms
        
        PORTCbits.RC2 = 1;      // SS igual a 1
        
        sendString("Pot1 = ");  // Tag
        potf1 = UARTsend*0.01961;
        sendfloat(potf1);     // Enviar valor de pot1
        
        
        sendString("Pot2 = ");  // Tag
        potf2 = UARTsend2*0.01961;
        sendfloat(potf2);
        
        sendString("Contador = ");
        sendhex(&contador);
        PORTB = contador;
        TXREG = 0x0D;       // Salto de linea
   
        __delay_ms(250);        // Delay de 250ms
    }
}

//******************************************************************************
//  Rutina de interrupcion
//******************************************************************************
void __interrupt() interrupcion(void){
    if(PIR1bits.RCIF){          // Bandera de recepcion de datos              
        PORTA = RCREG;          // RCREG a PORTA
        PIR1bits.RCIF = 0;      // Bajar la bandera
        
        if (RCREG != 0){
            contador = RCREG;
        }
    }
}

//******************************************************************************
//  funciones secundarias
//******************************************************************************
void config(void){
// Configuracion de entradas y salidas
    ANSEL = 0x00;               // Pines digitales
    ANSELH = 0x00;
    
    TRISA = 0x00;               // PORTA como salida
    TRISB = 0x00;               // PORTB como salida
    TRISCbits.TRISC2 = 0;       // Salida del SS
    TRISCbits.TRISC6 = 0;       // Salida de TX
    TRISCbits.TRISC7 = 1;       // Entrada de RX
    TRISD = 0x00;               // PORTD como salida
    TRISE = 0x00;               // PORTE como salida
    
    PORTA = 0x00;               // Limpiar puertos
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //  Configuracion del oscilador
    OSCCONbits.IRCF = 0b111;    // Frecuencia a 8MHZ
    OSCCONbits.SCS = 1;         // Habilitar reloj interno
    
    // Configuración de UART
    configUART();                                                               
    
    // Configuración de interrupciones
    INTCONbits.GIE = 1;         // Se habilitan las interrupciones globales
    INTCONbits.PEIE = 1;        // Se habilitan las interrupciones perifericas
    PIE1bits.RCIE = 1;          // Se habilitan las interupciones de recepción 
                                // de datos UART
}