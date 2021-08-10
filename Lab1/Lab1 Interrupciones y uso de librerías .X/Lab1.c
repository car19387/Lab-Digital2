//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: Lab9
 * Dispositivo: PIC16f887
 * Autor: Jefry Carrasco
 * Hardware: 
 * 1 PIC16F887
 * 1 display 7seg
 * Resistencias de 200ohms
 * 2 transistores NPN
 * 1 Potenciometro
 * 2 push button
 * Creado: 19 de julio, 2021
 * Modificado: 19 de julio, 2021 */  

//******************************************************************************
// Librerías incluidas
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
#include "ADC_a_7seg.h"         // Libreria personal

//******************************************************************************
// Configuración de PIC16f887
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

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
void setup();                   // Configuraciones del PIC
void config_io();               // Configurar entradas y salidas
void config_reloj();            // Configurar reloj
void config_adc();              // Configurar módulo ADC
void config_tmr0();             // Configurar TMR0
void config_int_enable();       // Enable interrupciones

//******************************************************************************
// Variables
//******************************************************************************
uint8_t contador;               // Contador binario
uint8_t decena;                 // Decena del display
uint8_t unidad;                 // Unidad del display
uint8_t multi;                  // Bandera de multiplecado
uint8_t bandera;                // Bandera para conversion

//******************************************************************************
//  Main
//******************************************************************************
void main(void){
    setup();
    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){     
        PORTC = contador;               // Contador es el valor de PORTC                 
        
        if(ADRESH>contador){            // Comparacion de contadores
            PORTEbits.RE1 = 1;          // Encender alarma                     
        }
        else{
            PORTEbits.RE1 = 0;          // Apagar alarma
        }
        
        if(bandera){                    // Si la bandera esta en alto
            multi++;                    // Aumentar variable para multiplexado
            if(multi>=2){               // Si sobrepasa las decenas
                multi = 0;              // Reinicio de multiplexado
            }
            
        switch(multi){
            case 0:
                PORTD = 0;
                PORTA = display(decena);    // Conversion de decenas
                PORTD = 0x01;               // Cambio de multiplexado
                break;
            case 1:
                PORTD = 0;
                PORTA = display(unidad);    // Conversion de unidades
                PORTD = 0x02;               // Cambio de multiplexado
                break;
            default:
                break;
        }
        bandera = 0;                        // Apagar la bandera
        }     
    }
}

//******************************************************************************
//  Interrupciones
//******************************************************************************
void __interrupt() interrupcion(void){
    if(INTCONbits.RBIF){                    // Interrupcion de IOCB
        if(PORTBbits.RB0==0){               // Si se presiona PB+
        contador++;                         // Aumentar contador
        }
        
        if(PORTBbits.RB1==0){               // Si se presiona PB+
        contador--;                         // Disminuir contador
        }
        
        INTCONbits.RBIF = 0;                // Se limpia la bandera del IOCB 
    }
        
    if(INTCONbits.T0IF){                    // Interrupcion de TMR0
        hexconv(ADRESH,&decena,&unidad);   // Conversion ADC a display 7seg
        if(!ADCON0bits.GO){                 // Si la bandera esta abajo
            ADCON0bits.GO = 1;              // Iniciar conversion ADC
        }
        bandera = 1;                        // Levantar bandera para display
        INTCONbits.T0IF = 0;                // Se limpia la bandera del TMR0        
    }
         
}

//******************************************************************************
//  COnfiguración
//******************************************************************************
void setup() {
    config_io();
    config_reloj();
    config_adc();
    config_tmr0();
    config_int_enable();
    return;
}

void config_io() {
    ANSEL = 0x10;               // RE0 como entrada analógica
    ANSELH = 0x00;
    
    TRISA = 0x00;               // Display 7seg
    TRISB = 0x03;               // Push Buttons
    TRISC = 0x00;               // Contador binario
    TRISD = 0x00;               // Multiplecado
    TRISE = 0x01;               // Pot y alarma
    
    PORTA = 0x00;               // Limpiar puertos
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    IOCBbits.IOCB = 0xFF;       // Hablititar iocb
}

void config_reloj() {
    OSCCONbits.IRCF = 0b111;    // Frecuencia a 8MHZ
    OSCCONbits.SCS = 1;         // Habilitar reloj interno
}

void config_adc() {
    ADCconfig(5,0);             // Canal = 5 y Justificado = izquierda
}

void config_tmr0() {
    OPTION_REGbits.T0CS = 0;    // Selecciona el reloj interno
    OPTION_REGbits.PSA  = 0;    // El prescaler seleccionado para el TMR0
    OPTION_REGbits.PS2  = 1;    
    OPTION_REGbits.PS1  = 1;    
    OPTION_REGbits.PS0  = 0;    // Prescaler a 1:128
    INTCONbits.T0IF = 0;        // Se limpia la bandera
}
void config_int_enable() {  
    INTCONbits.GIE = 1;         // Se habilitan las interrupciones globales
    INTCONbits.PEIE = 1;        // Se habilitan las interrupciones perifericas
    
    INTCONbits.RBIE = 1;        // Se habilita la interrupcion del IOCB
    INTCONbits.RBIF = 0;        // Se limpia la bandera del IOCB
    
    INTCONbits.T0IF = 0;        // Se habilita la interrupcion del TMR0
    INTCONbits.T0IE = 1;        // Se limpia la bandera del TMR0
}