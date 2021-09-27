/* Archivo: Juego_de_carrera.ino
 * Dispositivo: Tiva C TM4C123G
 * Autor: Angel Cuellar & Jefry Carrasco
 * Descripción: Juego de carrera iniciado por PB
 * cada jugador debera presionar 8 veces su PB para
 * ganar
 * Creado: 17 de septiembre de 2021
 * Modificado: de septiembre de 2021
 */

// Led RGB para semáforo
const int R = RED_LED;
const int G = GREEN_LED;
const int B = BLUE_LED;

// Leds de jugador 1
const int led1_1 = PB_5;
const int led2_1 = PB_0;
const int led3_1 = PB_1;
const int led4_1 = PE_4;
const int led5_1 = PE_5;
const int led6_1 = PB_4;
const int led7_1 = PA_5;
const int led8_1 = PA_6;

// Leds de jugador 2
const int led1_2 = PA_7;
const int led2_2 = PE_3;
const int led3_2 = PE_2;
const int led4_2 = PE_1;
const int led5_2 = PD_3;
const int led6_2 = PD_2;
const int led7_2 = PD_1;
const int led8_2 = PD_0;

// Leds de ganador
const int ledw1 = PB_3;
const int ledw2 = PC_4;

// Push Buttons
const int PBinicio = PC_5;
const int PB1 = PUSH1;
const int PB2 = PUSH2;

// Banderas y contadores
int flag = 0;
int cont1 = 0;
int cont2 = 0;


void setup() {
  // Se configuran los pines como entradas o salidas
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);

  pinMode(led1_1,OUTPUT);
  pinMode(led2_1,OUTPUT);
  pinMode(led3_1,OUTPUT);
  pinMode(led4_1,OUTPUT);
  pinMode(led5_1,OUTPUT);
  pinMode(led6_1,OUTPUT);
  pinMode(led7_1,OUTPUT);
  pinMode(led8_1,OUTPUT);

  pinMode(led1_2,OUTPUT);
  pinMode(led2_2,OUTPUT);
  pinMode(led3_2,OUTPUT);
  pinMode(led4_2,OUTPUT);
  pinMode(led5_2,OUTPUT);
  pinMode(led6_2,OUTPUT);
  pinMode(led7_2,OUTPUT);
  pinMode(led8_2,OUTPUT);

  pinMode(ledw1,OUTPUT);
  pinMode(ledw2,OUTPUT);

  pinMode(PBinicio,INPUT);
  pinMode(PB1,INPUT_PULLUP);
  pinMode(PB2,INPUT_PULLUP);

  // Se crean dos interrupciones para aumento de contador
  attachInterrupt(digitalPinToInterrupt(PB1),player1,FALLING);
  attachInterrupt(digitalPinToInterrupt(PB2),player2,FALLING);
  colorRGB(255,255,255);  
}

void loop() {
  // Condicion de inicio de semaforo
  if(digitalRead(PBinicio) == 1 && flag == 0){
    reseteo();
    inicioSemaforo();
  }
}

// Interupciones
void player1 (void){
  if (flag == 1){     
    delay(100);       // Tiempo de espera
    cont1 = cont1 + 1;  // Aumentar el contador
    // Encender las leds segun el contador
    if(cont1 == 1){digitalWrite(led1_1,HIGH);}
    if(cont1 == 2){digitalWrite(led2_1,HIGH);}
    if(cont1 == 3){digitalWrite(led3_1,HIGH);}
    if(cont1 == 4){digitalWrite(led4_1,HIGH);}
    if(cont1 == 5){digitalWrite(led5_1,HIGH);}
    if(cont1 == 6){digitalWrite(led6_1,HIGH);}
    if(cont1 == 7){digitalWrite(led7_1,HIGH);}
    if(cont1 == 8){
      digitalWrite(led8_1,HIGH);
      digitalWrite(ledw1,HIGH);
      cont1 = 0;
      cont2 = 0;
      flag = 0;
    }
  }
}

void player2 (void){
  if (flag == 1){
    delay(100);       // Tiempo de espera
    cont2 = cont2 + 1;  // Aumentar el contador
    // Encender las leds segun el contador
    if(cont2 == 1){digitalWrite(led1_2,HIGH);}
    if(cont2 == 2){digitalWrite(led2_2,HIGH);}
    if(cont2 == 3){digitalWrite(led3_2,HIGH);}
    if(cont2 == 4){digitalWrite(led4_2,HIGH);}
    if(cont2 == 5){digitalWrite(led5_2,HIGH);}
    if(cont2 == 6){digitalWrite(led6_2,HIGH);}
    if(cont2 == 7){digitalWrite(led7_2,HIGH);}
    if(cont2 == 8){
      digitalWrite(led8_2,HIGH);
      digitalWrite(ledw2,HIGH);
      cont1 = 0;
      cont2 = 0;
      flag = 0;
    }
  }
}

// Funciones secundarias
void reseteo(){
  colorRGB(255,255,255);  
  
  digitalWrite(led1_1,LOW);
  digitalWrite(led2_1,LOW);
  digitalWrite(led3_1,LOW);
  digitalWrite(led4_1,LOW);
  digitalWrite(led5_1,LOW);
  digitalWrite(led6_1,LOW);
  digitalWrite(led7_1,LOW);
  digitalWrite(led8_1,LOW);
  
  digitalWrite(led1_2,LOW);
  digitalWrite(led2_2,LOW);
  digitalWrite(led3_2,LOW);
  digitalWrite(led4_2,LOW);
  digitalWrite(led5_2,LOW);
  digitalWrite(led6_2,LOW);
  digitalWrite(led7_2,LOW);
  digitalWrite(led8_2,LOW);

  digitalWrite(ledw1,LOW);
  digitalWrite(ledw2,LOW);

  cont1 = 0;
  cont2 = 0;
}

void inicioSemaforo(void){
  // Verde, rojo, azul
  colorRGB(255,0,255);          // Luz roja
  delay(1000);                // Tiempo de espera
  colorRGB(15,0,200  );       // Luz amarilla
  delay(1000);                // Tiempo de espera
  colorRGB(0,255,255);          // Luz verde
  flag = 1;                   // Se levanta vandera para habilitar PB1&2
}

void colorRGB(int red, int green, int blue) {
  analogWrite(R, red);        // Se escribe el valor del rojo
  analogWrite(G, green);      // Se escribe el valor del verde
  analogWrite(B, blue);       // Se escribe el valor del azul
}
