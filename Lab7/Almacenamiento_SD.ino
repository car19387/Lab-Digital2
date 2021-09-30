/* Archivo: Almacenamient_SD.ino
 * Dispositivo: Tiva C TM4C123G
 * Autor: Jefry Carrasco
 * Descripción: Lectura de memoria microSD,
 * se implementa un menu para seleccionar la 
 * imagen a desplegar
 * Creado: 26 de septiembre de 2021
 * Modificado: 28 de septiembre de 2021
 */

// Librerias incluidas
#include <SPI.h>
#include <SD.h>
File archivo;     // Desclarar file de la libreria SD

// Variables
String archives[10];
const int chipSelect = PA_3; //PIN del chip select
int selec = 1;
bool flag1 = false;
bool flag2 = false;

void setup() {
  Serial.begin(9600);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  
  SPI.setModule(0);
  Serial.print("\nInitializing SD card...");
  pinMode(PA_3,OUTPUT);

  if(!SD.begin(chipSelect)){
    Serial.println(" Faill....");
    return;
    }
  Serial.println(" Succesful....");
  
  archivo = SD.open("/");       // Acceder a archivos de la SD
  ListaNombres(archivo);        // Se añaden los nosmbres de los archivos a la lista
  Serial.println("Archivos del menu:");
  for(int i=1; i < 5;i++){      // Imprimir los nombres de los archivos en la SD
    Serial.println(archives[i]);
    }
  Serial.println("Indicar archivo a imprimir: \n" ); 
  Serial.println("Imprimir el archivo " + archives[selec] + "?\n");
}


void loop() {
  if(digitalRead(PUSH1) == LOW){ 
    flag1 = true;
    }
    
  if(digitalRead(PUSH1)== HIGH && flag1 == true){
    selec ++;
    if (selec >= 4){
      selec = 1;
      }
    Serial.println("Imprimir el archivo " + archives[selec] + "?\n");
    flag1 = false;
    }   
    
  if(digitalRead(PUSH2) == LOW){
    flag2 = true;
    }
    
   if(digitalRead(PUSH2)== HIGH && flag2 == true){
    char impresion[20]; 
    archives[selec].toCharArray(impresion,20); 
    imprimirArchivo(impresion); 
    Serial.print("\n\n\n");
    flag2 = false;
    }
}

void ListaNombres(File archivo){
  int n = 0; 
  while(true){
    File dummy = archivo.openNextFile(); 
    if(!dummy){ break;} 
    archives[n] = dummy.name(); 
    n++; 
    }
  }

void imprimirArchivo(const char *nombre){
  File archivo; 
  archivo = SD.open(nombre,FILE_READ);
  if (archivo){                   // Si esta disponible
    while(archivo.available()){   // Imprimir el contenido
      Serial.write(archivo.read());
      }
      archivo.close();
    }
  else{   
    Serial.println("error de apertura");
    }
  }
