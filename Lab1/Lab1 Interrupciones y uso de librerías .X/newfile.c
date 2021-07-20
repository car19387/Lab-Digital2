//******************************************************************************
//  Encabezado
//******************************************************************************
/* Archivo: Lab9
 * Dispositivo: PIC16f887
 * Autor: Jefry Carrasco
 * Descripción: 
 *  
 * 
 * Hardware: 
 * 1 PIC16F887
 * 1 display 7seg
 * Resistencias de 200ohms
 * 2 transistores NPN
 * 1 Potenciometro
 * 2 push button
 * Creado: 19 de julio, 2021
 * Modificado:  de julio, 2021 */  

//******************************************************************************
// Librerías incluidas
//******************************************************************************   
 #include <xc.h>
 #include <stdint.h>

//******************************************************************************
// Configuración de PIC16f887
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
// Directivas del preprocesador
//******************************************************************************
#define _XTAL_FREQ 8000000      // Libreria para delay

//******************************************************************************
// VAriables
//******************************************************************************
uint8_t unidad_display = 0; 
uint8_t decena_display = 0;

//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void setup();               // Configuraciones del PIC
void config_io();           // Configurar entradas y salidas
void config_reloj();        // Configurar reloj
//void config_adc();          // Configurar módulo ADC
void config_int_enable();   // Enable interrupciones

//******************************************************************************
// Main
//******************************************************************************
void main(void) {
    setup();                // Llamar función de configuraciones
    
    while(1){               // loop principal
         
    }
    return;
}

//******************************************************************************
// Interrupciones
//******************************************************************************
void __interrupt() isr(void){
    PORTD = PORTD++;
    if(INTCONbits.RBIF){            // Interrupción PORTB
        
        if (PORTBbits.RB0 == 0){    // Si se preciona PB+
            PORTC++;                // Incrementar PORTA
        }
        if(PORTBbits.RB1 == 0) {    // Si se preciona PB-
            PORTC--;                // Decrementar PORTA
        }
        INTCONbits.RBIF = 0;        //Limpiar la bandera
    }
    return;
}

//******************************************************************************
// Configuraciones
//******************************************************************************
void setup() {
    config_io();
    config_reloj();
    //config_adc();
    config_int_enable();
    return;
}

void config_io() {
    ANSELH = 0x00;  // Pines digitales
    ANSEL = 0x00;   // 
    
    TRISA = 0x00;   // Display 7seg
    TRISB = 0xFF;   //¨Push Button
    TRISC = 0x00;   // Contador
    TRISD = 0x00;   // Multiplexado
    TRISE = 0xFE;   // Alarma y pot
               
    PORTA = 0x00;   // Se limpian los puertos    
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    OPTION_REGbits.nRBPU = 1;
    WPUBbits.WPUB = 0xFF;
}
 
void config_reloj() {
    OSCCONbits.IRCF2 = 1;   //Frecuencia a 8MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;     // Habilitar reloj interno
}
    
/*void config_adc() {   
    ADCON1bits.ADFM = 0;    // Justifiación a la izquierda
    ADCON1bits.VCFG0 = 0;   // Voltaje de referencia Vss y Vdd
    ADCON1bits.VCFG1 = 0;   
    
    ADCON0bits.ADCS = 2;    // ADC clokc Fosc/32
    ADCON0bits.CHS = 0;     // Canal 0 selecionado
    __delay_us(50);
    ADCON0bits.ADON = 1;    // Enecender módulo ADC
}*/

void config_int_enable() {   
    INTCONbits.GIE = 1;     // Se habilitan las interrupciones globales
    INTCONbits.PEIE = 1;    // Se habilitan las interrupciones perifericas
    //PIE1bits.ADIE = 1;      // Se habilita la interrupcion del ADC
    //PIR1bits.ADIF = 0;      // Se limpia la bandera del ADC
    INTCONbits.RBIE = 1;    // Se habilitan las interrupciones del PORTB
    INTCONbits.RBIF = 0;    // Se limpia la bandera del IOC
}
