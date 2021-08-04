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
#include "ADC.h"
#include "SPI.h"

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
uint8_t pot1,pot2;
char dato;

//******************************************************************************
//  loop principal
//******************************************************************************
void main(void) {
    config();
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){
      PORTB = pot1;             // Valor de Pot1 se guarda en PORTB
      PORTD = pot2;             // Valor de Pot2 se guarda en PORDB
    }
}

//******************************************************************************
//  Rutina de interrupcion
//******************************************************************************
void __interrupt() interrupcion(void){
    if(PIR1bits.ADIF){                      // Bandera de ADC
        ADCON0bits.CHS0 = ~ADCON0bits.CHS0; // Cambio de canal
        PIR1bits.ADIF = 0;                  // Se apaga la bandera
        if(!ADCON0bits.CHS0)pot1 = ADRESH;  // Se guarda el valor de conversión
        else pot2 = ADRESH;                 // Se guarda el valor de conversión
    }
    
    if(INTCONbits.T0IF){                    // Bandera de TMR0
        if(!ADCON0bits.GO)ADCON0bits.GO = 1;// Iniciar conversión
        INTCONbits.T0IF = 0;                // Apagar bandera
    }
    
    if(PIR1bits.SSPIF){                     // Bandera de SPI
        dato = readSPI();                   // Leer dato del maestro
        if(dato == '1') sendSPI(pot1);      // Enviar dato del pot1
        else if(dato == '2') sendSPI(pot2); // Enviar dato del pot2
        PIR1bits.SSPIF = 0;                 // Apagar bandera
    }
    
}

//******************************************************************************
//  funciones secundarias
//******************************************************************************
void config(void){
// Configuracion de entradas y salidas
    ANSEL = 0x03;               // AN0 y AN1 entradas analogicas
    ANSELH = 0x00;
    
    TRISAbits.TRISA0 = 1;       // Pot1
    TRISAbits.TRISA1 = 1;       // Pot2
    TRISB = 0x00;               // PORTB como salida
    TRISD = 0x00;               // PORTD como salida
    TRISE = 0x00;               // PORTE como salida
    
    PORTA = 0x00;               // Limpiar puertos
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
// Configuracion del oscilador
    OSCCONbits.IRCF = 0b111;    // Frecuencia a 8MHZ
    OSCCONbits.SCS = 1;         // Habilitar reloj interno
    
// Configuracion del TMR0
    OPTION_REGbits.T0CS = 0;    //Timmer 0 a FOSC y Prescalador asignado
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.PS2  = 1;    //valor del prescalador
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS0  = 0;
    
// Configuracion del ADC
    ADCconfig(0,0);             // Canal 0 y justificado a la izquierda
    PIE1bits.ADIE = 1;          // Se habilitan las interrupciones del ADC
    PIR1bits.ADIF = 0;          // Se apaga la bandera del ADC
    
// Configuración de interrupciones
    INTCONbits.GIE = 1;         // Se habilitan las interrupciones globales
    INTCONbits.PEIE = 1;        // Se habilitan las interrupciones perifericas
    INTCONbits.T0IE = 1;        // Se habilitan las interrupciones del TMR0
    INTCONbits.T0IF = 0;        // Se apaga la bandera del TMR0
    PIE1bits.SSPIE = 1;         // Se habilitan las interrupciones del SPI
    PIR1bits.SSPIF = 0;         // Se apaga la bandera del SPI
}