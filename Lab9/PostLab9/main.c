//**************************************************************************************************************
/*Autor: Jefry Carrasco
 *Dispositivo: Launchpad Tiva C
 *Hardware: Tiva C
 *Creado: Noviembre 03, 2021
 *Ultima modificacion: Noviembre 03, 2021
 */

//**************************************************************************************************************
// Librerias
//**************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

//**************************************************************************************************************
// Variables Globales
//**************************************************************************************************************
uint32_t ui32Period;

//**************************************************************************************************************
// Código Principal
//**************************************************************************************************************
int main(void)
{
    // Configuración del oscilador externo, usando PLL y teniendo una frecuencia de 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Se habilita el reloj para el puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Se establecen como salidas el puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Se habilita el reloj para el temporizador
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Configuración del Timer 0 como temporizador periodico
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Se calcula el período para el temporizador (1 seg - 50% duty cycle)
    ui32Period = (SysCtlClockGet()) / 2;

    // Establecer el periodo del temporizador
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);

    // Se habilita la interrupción por el TIMER0A
    IntEnable(INT_TIMER0A);
    // Se establece que exista la interrupción por Timeout
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Se habilitan las interrupciones Globales
    IntMasterEnable();
    // Se habilita el Timer
    TimerEnable(TIMER0_BASE, TIMER_A);

    //**********************************************************************************************************
    // Loop Principal
    //**********************************************************************************************************
    while (1)
    {
    }
}

//**************************************************************************************************************
// Handler de la interrupción del TIMER 0 - Recordar modificar el archivo tm4c123ght6pm_startup_css.c
//**************************************************************************************************************
void Timer0IntHandler(void)
{
    //Limpia la interrupcion
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //Toggle del led verde
    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
    }
}
