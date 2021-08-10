/* File:   I2C.h
 * Author: Jefry Alexander Carrasco Yac
 * Creado: 6 de agosto, 2021
 * Modificado:  de julio, 2021
 */

#ifndef I2C_H
#define	I2C_H

#include <xc.h>  
#include <stdint.h>
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

// Inicio del master en I2C
void MasterInit_I2C(unsigned long frec);

// Iniciar el esclavo en I2C
void SlaveInit_I2C(uint8_t address);

// Tiempo de espera para realizar las operaciones
void waitCondition(void);

// SDA y SCL realizan la condicion de inicio
void MasterStart_I2C(void);

// SDA y SCL realizan la condicion de parada
void MasterStop_I2C(void);

// SDA y SCL realizan la condicion de reinicio de COM
void MasterRepeatS_I2C(void);

// Coloca el dato en el buffer y luego lo envia
void MasterSend_I2C(uint8_t dato);

// Espera entre operaciones y coloca los valores para indicar que se va a recibir un valor*/
void MasterReceive_I2C(uint8_t *valor);

#endif	/* I2C_H */

