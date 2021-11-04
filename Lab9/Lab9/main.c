//**************************************************************************************************************
/*Autor: Jefry Carrasco
 *Dispositivo: Launchpad Tiva C
 *Hardware: Tiva C
 *Creado: Noviembre 03, 2021
 *Ultima modificacion: Noviembre 03, 2021
 */

//**************************************************************************************************************
// Librerías
//**************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#define XTAL 16000000

//**************************************************************************************************************
// Variables Globales
//**************************************************************************************************************
uint32_t i = 0;
uint8_t flag=0;
uint8_t n;

//**************************************************************************************************************
// Prototipos de Funciones
//**************************************************************************************************************
void Button(void);
void delay(uint32_t msec);
void delay1ms(void);

//**************************************************************************************************************
// Función Principal
//**************************************************************************************************************
int main(void)
{
    // Se setea oscilador externo de 16MHz
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);  //16MHz

    // Se asigna reloj a puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //Se establece como entrada el pin 4
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

    // Se establecen como salidas los pines 1, 2 y 3 del puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    //Se configura como weak pull-up
    uint32_t ui32Strength = GPIO_STRENGTH_2MA;
    uint32_t ui32PinType = GPIO_PIN_TYPE_STD_WPU;

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, ui32Strength, ui32PinType);

    //Se configura la interrupcion
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOIntRegister(GPIO_PORTF_BASE, Button);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);

   //**********************************************************************************************************
   // Loop Principal
   //**********************************************************************************************************
   while (1)
    {
        // Se enciende la led roja
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1 | 0);

        if(flag){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
            GPIO_PIN_3 | 0);
            SysCtlDelay(8000000);   //delay(1500);

            for (n = 0; n < 3; n++) //Parpadeo 3 veces
               {
                   // Se apagan todos los pines
                   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
                   //delay(500);
                   SysCtlDelay(2666666);

                   // Se encienden la led verde
                   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_3 | 0);
                   //delay(500);
                   SysCtlDelay(2666666);
               }
            //Se enciende la led amarilla
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
            GPIO_PIN_1 |GPIO_PIN_3 | 0);
            //delay(1500);
            SysCtlDelay(8000000);
            flag = 0; //Se limpia la bandera
        }
    }
}

//**************************************************************************************************************
// Función para hacer debounce
//**************************************************************************************************************
void Button(void){
    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4) { //Sucede la interrupcion
        flag = 1; //Se enciende la bandera
    }
}

//**************************************************************************************************************
// Función para hacer delay en milisegundos
//**************************************************************************************************************
void delay(uint32_t msec)
{
    for (i = 0; i < msec; i++)
    {
        delay1ms();
    }
}

//**************************************************************************************************************
// Función para hacer delay de 1 milisegundos
//**************************************************************************************************************
void delay1ms(void)
{
    SysTickDisable();
    SysTickPeriodSet(16000);
    SysTickEnable();
    while ((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) == 0);

}
