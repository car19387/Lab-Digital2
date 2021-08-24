//******************************************************************************
//  Encabezado
//******************************************************************************
/* File: P1 Slave1 TCS230.c
 * Device: PIC16F887
 * Author: Angel Cuellar & Jefry Carrasco
 * Descripción: Sensor de color TCS230 para identificar gaseosas, control de 
 * servos para clasificación. Comunicación I2C para envio de datos.
 * Creado: 17 de agosto, 2021
 * Modificado:  de agosto, 2021 */

//******************************************************************************
// Librerías incluidas
//******************************************************************************
#include <xc.h>
#include <stdint.h>
#include "I2C.h"

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
#define _XTAL_FREQ 8000000      // Para delay

//******************************************************************************
// Variables
//******************************************************************************
uint8_t CC = 10;                // Cantidad de Coca Cola
uint8_t Sp = 20;                // Cantidad de Sprite
uint8_t b;                      // Bandera para seleccion de dato
uint8_t z;                      // Envio de datos

//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void setup(void);               // Configuración

//******************************************************************************
// Main
//******************************************************************************
void main(void) {
    setup();                    // Iniciar configuraciones
    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){

    }
    return;
}

//******************************************************************************
// Interrupciones
//******************************************************************************
void __interrupt()isr(void){
    if(PIR1bits.SSPIF == 1){            
        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Leer valor del buffer y limpiarlo
            SSPCONbits.SSPOV = 0;       // Se limpia la bandera de overflow
            SSPCONbits.WCOL = 0;        // Se limpia el bit de colision
            SSPCONbits.CKP = 1;         // Se habilita SCL
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            z = SSPBUF;                 // Leer valor del buffer y limpiarlo
            PIR1bits.SSPIF = 0;         // Limpia la bandera de SSP
            SSPCONbits.CKP = 1;         // Habilita los pulsos del reloj SCL
            while(!SSPSTATbits.BF);     // Esperar que la recepcion se realice
            b = SSPBUF;                 // Guardar el valor del buffer
            __delay_us(250);      
        }
        
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;                 // Leer valor del buffer y limpiarlo
            BF = 0;
            if(b == 0){                 // Si se pide el primer dato
                SSPBUF = CC;            // Escribe valor de la variable al buffer
            }
            if(b == 1){                 // Si se pide el segundo dato
                SSPBUF = Sp;            // Escribe valor de la variable al buffer
            }
            SSPCONbits.CKP = 1;         // Habilita los pulsos del reloj SCL
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        PIR1bits.SSPIF = 0;    
    }
}

//******************************************************************************
// Configuraciones
//******************************************************************************
void setup(void){
    // Configuración de Puertos
    ANSEL = 0x01;               // RA0 como entrada analógica
    ANSELH = 0x00;
    
    TRISA = 0x01;               // Fotoresistencua en RA0
    TRISB = 0x00;
    TRISC = 0x08;               // Señal de reloj
    TRISD = 0x00; 
    TRISE = 0x00; 
    
    PORTA = 0x00;               // Limpiar puertos
    PORTB = 0x00;
    PORTC = 0x00;    
    PORTD = 0x00;
    PORTE = 0x00;
    
    // Configuracion del oscilador
    OSCCONbits.IRCF = 0b111;    // Frecuencia a 8MHZ
    OSCCONbits.SCS = 1;         // Habilitar reloj interno
    
    //Configurar la interrupcion
    INTCONbits.GIE = 1;         // Se habilitan las interrupciones globales
    INTCONbits.PEIE = 1;        // Se habilitan las interrupciones perifericas
    I2C_Slave_Init(0x60);       // Direccion del esclavo                       
}