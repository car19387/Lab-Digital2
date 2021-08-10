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
#include "I2C.h"
#include "LCD.h"

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
void floTochar(const float valor,unsigned char *conv);
void hexTochar(uint8_t valor,unsigned char *conv);
void hexTocharNEG (uint8_t valor,unsigned char *conv);
void division(uint8_t conteo,uint8_t* un,uint8_t* dec);
void divisiondecimal(uint8_t conteo,uint8_t* un,uint8_t* dec,uint8_t* cent);

//******************************************************************************
// Variables
//******************************************************************************
uint8_t DatS1;
uint8_t DatS2;
uint8_t DatS3;
float lect1;
unsigned char disp1[3];
unsigned char disp2[3];
unsigned char disp3[3];

//******************************************************************************
//  Main
//******************************************************************************
void main(void) {
    config();               // Configuración inicial
    initLCD();              // Inicialización de LCD
    MasterStart_I2C();      // Inicio de I2C
    MasterSend_I2C(0x90);   // Direccion del sensor de temperatura
    MasterSend_I2C(0xAC);   // Acceder al registro de configuracion
    MasterSend_I2C(0x00);   // Le indica que es una conversion continua
    MasterRepeatS_I2C();
    MasterSend_I2C(0X90);   // Ahora se comunica con lectura a otro registro
    MasterSend_I2C(0XEE);   // Para que la ocnversion de temperatura sea continua
    MasterStop_I2C();       // Se detiene la comunicacion
    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){  
        // Pedir dato al slave 1
        MasterStart_I2C();          //Inicia la comunicacion
        MasterSend_I2C(0X21);       //direccion del que se va a leer
        MasterReceive_I2C(&DatS1);  // Recibe el dato
        PORTB = DatS1;              // Se imprime el dato en el PORTB
        MasterStop_I2C();           //termina la comunicacion
        
        // Pedir dato al slave 2
        MasterStart_I2C();          // Inicia la comunicacion
        MasterSend_I2C(0X31);       // Direccion del que se va a leer
        MasterReceive_I2C(&DatS2);  // Recibe dato
        MasterStop_I2C();//termina la comunicacion
        
        // Pedir dato al sensor de temperatura
        MasterStart_I2C();
        MasterSend_I2C(0X9A);           //direccion del sensor en escritura
        MasterSend_I2C(0X0);           //para que aliste la temeperatura en lectura
        MasterRepeatS_I2C();            //reinicia la com
        MasterSend_I2C(0X9B);           //ahora como una lectura
        MasterReceive_I2C(&DatS3);      //guarda el valor de la temperatura
        MasterStop_I2C();
        
        lect1 = (float)(0.01961)*(DatS1);   //pasar el valor leido a flotante
        
        // Despliegue de valores luego de la lectura
        floTochar(lect1,&disp1);
        hexTochar(DatS2,&disp2);
        hexTocharNEG(DatS3,&disp3);
        cursorLCD(1,1);
        LCDstring("S1:   S2:   S3:");
        cursorLCD(2,1);
        dispCHAR(disp1[0]+48);
        dispCHAR('.');
        dispCHAR(disp1[1]+48);
        dispCHAR(disp1[2]+48);
        dispCHAR('V');
        cursorLCD(2,7);
        dispCHAR(disp2[2]+48);
        dispCHAR(disp2[1]+48);
        dispCHAR(disp2[0]+48);
        cursorLCD(2,12);
        if(DatS3<=128) dispCHAR(disp3[2]+48); //si despliega el numero
        else dispCHAR(disp3[2]); //despliega el signo menos
        dispCHAR(disp3[1]+48);
        dispCHAR(disp3[0]+48);
        dispCHAR(223); //desplegar el ° para los °C
        dispCHAR('C');
    }
}

//******************************************************************************
// Configuración    
//******************************************************************************
void config(void){
    // Configuración de Puertos
    ANSEL = 0x00;               // Pines digitales
    ANSELH = 0x00;
    
    TRISA = 0x00;               // PORTA como salida
    TRISB = 0x00;               // PORTB como salida
    TRISD = 0x00;               // PORTC como salida
    TRISE = 0x00;               // PORTD como salida

    PORTA = 0x00;               // Limpiar puertos
    PORTB = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //  Configuracion del oscilador
    OSCCONbits.IRCF = 0b111;    // Frecuencia a 8MHZ
    OSCCONbits.SCS = 1;         // Habilitar reloj interno
    MasterInit_I2C(100000);     // Comunicación I2C a 100KHz
}

void floTochar(const float valor,unsigned char *conv){
    uint8_t entero;
    uint8_t decimal;
    float temp;
    unsigned char digdecimal[2];
    /*Toma un valor tipo float y pasa su entero a una variable, el valor 
     * restante lo opera para pasar las decimas a enteros y luego separarlos
     * en 2 datos, para poder representarlos luego en el puerto serial enviando
     * un caracter a la vez.
     */
    entero = valor;
    digdecimal[2] = entero;
    temp = valor-(float)entero;
    decimal = (temp*100);
    division(decimal,&digdecimal[0],&digdecimal[1]);
    conv[0] = entero;
    conv[1] = digdecimal[1];
    conv[2] = digdecimal[0];
}

// Operación de división
void division(uint8_t conteo,uint8_t* uni,uint8_t* dec){
    uint8_t div = conteo;
    *uni =   0;
    *dec =  0;
    
    while (div >= 10){
    *dec = div/10;
    div = div - (*dec)*(10); }
    
    *uni = div;
}

void hexTochar(uint8_t valor,unsigned char *conv){ 
    uint8_t centena;
    uint8_t decena;
    uint8_t unidad;
    
    divisiondecimal(valor,&unidad,&decena,&centena);
    conv[0]= unidad;
    conv[1]= decena;
    conv[2]= centena;

}

void hexTocharNEG (uint8_t valor,unsigned char *conv){ 
    /*Funcion especial implementada para el sensor, donde se pueden leer los 
     valores de temperatura positivos y negativos agregando de forma directa
     el signo en la lectura*/
    uint8_t centena;
    uint8_t decena;
    uint8_t unidad;
    uint8_t temporal;
    if(valor <= 128){
        divisiondecimal(valor,&unidad,&decena,&centena);
        conv[0]= unidad;
        conv[1]= decena;
        conv[2]= centena;
    }
    else{
        temporal = 256 - valor;
        divisiondecimal(temporal,&unidad,&decena,&centena);
        conv[0]= unidad;
        conv[1]= decena;
        conv[2]= '-';
    }
}

// Divición con decimales
void divisiondecimal(uint8_t conteo,uint8_t* uni,uint8_t* dec,uint8_t* cent){
    uint8_t div = conteo;
    *uni =   0;
    *dec =  0;
    *cent = 0;
    
    while(div >= 100){
    *cent = div/100;
    div = div - (*cent)*(100); }
    
    while (div >= 10){
    *dec = div/10;
    div = div - (*dec)*(10); }
    
    *uni = div;
}
