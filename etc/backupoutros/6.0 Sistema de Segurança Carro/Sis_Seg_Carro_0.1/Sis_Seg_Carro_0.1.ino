#include "DHT.h"


char LER;
bool LIGADO;
bool FAROL;
float U; 
float T; 
int LDR;
int PIR;
int pinoLDR = A2;
int pinoPIR = 7;
//int ledVERMELHO = 2; //GENTE DENTRO
//int ledVERDE = 3;    //TEMP E UMI ALTA, levantar 
//int farol = 4;    //DESLIGAR FAROL
DHT dht = DHT(A0, DHT11);

void setup(){
  Serial.begin(9600);
  dht.begin();
  pinMode(2, OUTPUT); //VERDE, GENTE
  pinMode(3, OUTPUT); //VERMELHO, TEMPERATURA 
  pinMode(4, OUTPUT); //BRANCO, FAROL 
  LIGADO = false;
  FAROL = false;
}

void debug(int valor){
  if(valor == 1){
    Serial.print("Umidade: ");
    Serial.println(U);
    Serial.print("Temperatura: ");
    Serial.println(T);
    Serial.print("Valor LDR: ");
    Serial.println(LDR);
    Serial.println("----------------------------------------------------");
  } else {
  }
}

void loop(){
  debug(0);
  LDR = analogRead(pinoLDR);
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
    PIR = digitalRead(pinoPIR);
    if(FAROL == true){
      digitalWrite(4, HIGH);
      if(LDR < 850){
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
    if(PIR > 0){
      Serial.println("Animal ou criança dentro do carro!!!");
      digitalWrite(2, HIGH);
      if(T > 29){
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
      digitalWrite(4, HIGH);
      if(LDR < 850){
        Serial.println("Farol está ligado!!!");
      }else{
        Serial.println("Farol está queimado!!!");
      }
    }
    if(FAROL == false){
      digitalWrite(4, LOW);
      Serial.println("Farol está desligado!!!");
    }
  }
  delay(2000);
}
