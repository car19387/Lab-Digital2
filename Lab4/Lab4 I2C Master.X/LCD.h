/* File:   LCD.h
 * Author: Jefry Alexander Carrasco Yac
 * Creado: 6 de agosto, 2021
 * Modificado:  de julio, 2021
 */

#ifndef LCD_H
#define	LCD_H

#include <xc.h>

// Inicializacion de LCD
void initLCD(void);

// Escribir en la LCD
void dispCHAR(char b);

// Mover el cursor
void cursorLCD(uint8_t fila, uint8_t columna);

// Enviar comandos a la LCD
void comandoLCD(uint8_t cmd);

// Limpia la pantalla
void ClearLCD(void);

// Enviar un string a la LCD
void LCDstring(unsigned char* mensaje);

#endif	/* LCD_H */

