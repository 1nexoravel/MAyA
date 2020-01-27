#include "DHT.h"

char LER;
bool LIGADO;
bool FAROL;
float U; 
float T; 
int LDRfarol;
int LDRinterno;
int PIR;
int pinoLDRfarol = 14;
int pinoLDRinterno = 27;
int pinoPIR = 26;
int pinoLEDfarol = 15; //15 2 0 4
int pinoLEDinterno = 2; //15 2 0 4
DHT dht = DHT(13, DHT11);

void setup(){
  Serial.begin(9600);
  dht.begin();
  pinMode(32, OUTPUT); //VERDE, GENTE
  pinMode(33, OUTPUT); //VERMELHO, TEMPERATURA 
  pinMode(pinoLEDinterno, OUTPUT); //AZUL, LATERNA INTERNA
  pinMode(pinoLEDfarol, OUTPUT); //BRANCO, FAROL
  pinMode(pinoPIR, INPUT); 
  pinMode(pinoLDRfarol, INPUT); 
  pinMode(pinoLDRinterno, INPUT);
  LIGADO = false;
  FAROL = false;
}

void debug(int valor){
  if(valor == 1){
    Serial.print("Umidade: ");
    Serial.println(U);
    Serial.print("Temperatura: ");
    Serial.println(T);
    Serial.print("Valor LDR farol: ");
    Serial.println(LDRfarol);
    Serial.print("Valor LDR interno: ");
    Serial.println(LDRinterno);
    Serial.print("Valor PIR: ");
    Serial.println(PIR);
    Serial.println("----------------------------------------------------");
  } else {
  }
}

void loop(){
  debug(1);
  LDRfarol = analogRead(pinoLDRfarol);
  LDRinterno = analogRead(pinoLDRinterno);
  PIR = digitalRead(pinoPIR);
  LER = Serial.read();
  if(LER == 'L'){
    LIGADO = true;
    Serial.println("Ligou o carro.");
    }
  if(LER == 'D'){
    LIGADO = false;
    Serial.println("Desligou o carro.");
    }
  if(LER == '1'){
    FAROL = true;
    Serial.println("Ligou o farol.");
  }
  if(LER == '0'){
    FAROL = false;
    Serial.println("Desligou o farol.");
  }
   
  if(LIGADO == false){ 
    Serial.print("Carro está desligado. -> "); 
    U = dht.readHumidity();
    T = dht.readTemperature();
    
    if(FAROL == true){
      digitalWrite(pinoLEDfarol, HIGH);
      if(LDRfarol < 850){
          Serial.println("Farol está ligado!!!");
          delay(1000);
          Serial.println("Desligando o farol em 5 segundos.");
          delay(5000);
          digitalWrite(4, LOW);
          FAROL = false;
          Serial.println("Farol está desligado!!!");
        }else{
        Serial.println("");
        }
      }else{
        Serial.println("");
    }
    if(PIR < 300){
      Serial.println("Animal ou criança dentro do carro!!!");
      digitalWrite(2, HIGH);
      if(T > 28){
        Serial.println("Temperatura alta!!!");
        digitalWrite(3, HIGH);
        Serial.println("Descendo as janelas.");    
          }
      }
  }else{
    Serial.print("Carro está ligado. -> ");
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    if(FAROL == true){
      digitalWrite(pinoLEDfarol, HIGH);
      if(LDRfarol < 850){
        Serial.println("Farol está ligado!!!");
      }else{
        Serial.println("Farol está queimado!!!");
      }
    }
    if(FAROL == false){
      digitalWrite(pinoLEDfarol, LOW);
      Serial.println("Farol está desligado!!!");
    }
  }
  delay(5000);
}
