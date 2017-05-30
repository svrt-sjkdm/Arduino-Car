
#include <LiquidCrystal.h>

#define trigPin 9                // Pin donde se envian 8 pulsos ultrasonicos
#define echoPin 8                // Pin donde se reciben los pulsos enviados
#define speedSound 0.0344

LiquidCrystal lcd(7,6,5,4,3,2);   // Creacion objeto LC con los pines que se utilizan

int maxDis = 8, turnTo = 0, dir = 0;
float distancia;
unsigned long seconds = 0;
boolean control = false, autonomo = false;
char x;

void printDistance();
void printDirection(String dir);
float medirDistancia();
void evadeObstaculo();

void setup() {
  DDRB = B111110;
  lcd.begin(16, 2);               // La plantalla del LCD es de 16x2
  Serial.begin(9600);             // Iniciar comunicacion serial a 9600 bauds
} 

void loop() {

if(Serial.available()) {
  x = Serial.read();
  switch(x) {
    case 'A':      // Activar modo manual
      control = true;
      autonomo = false;
      lcd.clear();
    break;
    case 'B':     // Activar modo autonomo
      autonomo = true;
      control = false;
      lcd.clear();
    break;
    default:
    break;
  }
}

// Se maneja con el celular
if(control == true and autonomo == false) {             
  if(Serial.available()) {
    x = Serial.read();
    distancia = medirDistancia();
    switch(x) {
      case 'C':    // Adelante
        PORTB = B010100;
        printDirection("Adelante");
      break;
      case 'D':    // Atras
        PORTB = B101000;
        printDirection("Atras");
      break;
      case 'E':    // Izquierda
        PORTB = B011000;
        printDirection("Izquierda");
      break;
      case 'F':    // Derecha
        PORTB = B100100;
        printDirection("Derecha");
      break;
      case 'G':   // Alto
        PORTB = B000000;  
        printDirection("Alto");
      break;
      default:
      break;
    }
  }
}
// Modo detector de obstaculos
else if(autonomo == true and control == false)   {    
    dir = random(0,5);
    seconds = random(50,100);
    switch(dir) {
    // Adelante
    case 0:      
      for(unsigned long i = 0 ; i <=  seconds; i++) {
        printDirection("Adelante");
        distancia = medirDistancia();
        if(distancia > maxDis) {
          PORTB = B010100; 
        }
        else {
            evadeObstaculo();
            break;
        }
        Serial.println(i);
      }
    break;
    // Izquierda
    case 1:   
      for(int i = 0 ; i <=  50; i++) {
        printDirection("Izquierda");
        distancia = medirDistancia();
        if(distancia > maxDis) {
          PORTB = B011000;
        }
        else {
            evadeObstaculo();
            break;
        }
      }
    break;
    // Derecha
    case 2:                   
      for(int i = 0 ; i <= 50; i++) {
        printDirection("Derecha");
        distancia = medirDistancia();
        if(distancia > maxDis) {
          PORTB = B100100;
        }
        else {
            evadeObstaculo();
            break;
        }
      }
    break;
    default:
    break;
    }
  }
else {
  lcd.print("Proyecto");
  lcd.setCursor(0,1);
  lcd.print("L Formales y A  ");
}
  }

void printDistance() {
  lcd.clear();
  lcd.setCursor(0,0);
   lcd.print("Distancia: ");
   lcd.setCursor(0,1);
   lcd.print(round(distancia));
   lcd.print(" cm"); 
}
void printDirection(String dir) {
   lcd.clear();
   lcd.print(dir);
}

float medirDistancia() {
  float duracion = 0, distancia = 0;
  // Envia una señal baja durante 2us
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Envia una señal alta durante 10us
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duracion = pulseIn(echoPin, HIGH);
  distancia = (duracion/2) * speedSound;
  return distancia;
}

void evadeObstaculo() {
  lcd.clear();
  lcd.print("Obstaculo");
  lcd.setCursor(0,1);
  lcd.print("Detectado");
  PORTB = B000000;     // Se detiene 1 segundo
  delay(1000);
  // Gira a una direccion aleatoria
  turnTo = random(0,1);
  if(turnTo == 0) {
    for(int i = 0; i <= 50; i++) 
      PORTB = B011000;   // Gira izquierda
  }
  else {
    for(int i = 0; i <= 50; i++) 
      PORTB = B100100;   // Gira derecha
  }
  lcd.clear();
}

