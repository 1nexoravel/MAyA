#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#include "DHT.h"

char auth[] = "zwLH8Wds5_mmLxfSrmNppgiwMv5bCaTs";
char LER;
bool DEBUG;
bool LIGADO;
bool FAROL;
bool LUZ;
bool FLUXO;
float U; 
float T; 
int LDRfarol;
int LDRinterno;
int PIR;
int pinoLDRfarol = 34;
int pinoLDRinterno = 35;
int pinoPIR = 32;
int pinoLEDfarol = 15; //15 2 0 4
int pinoLEDinterno = 2; //15 2 0 4
int pinoLEDdentro = 0;
int pinoLEDjanela = 4;
int pinoLEDtemperatura = 4;
DHT dht = DHT(13, DHT11);
BlynkTimer timer;

void setup(){
  Serial.begin(9600);
  Blynk.setDeviceName("Blynk Poderoso");
  Blynk.begin(auth);
  timer.setInterval(1000L, sendSensor);
  dht.begin();
  pinMode(pinoLEDdentro, OUTPUT); //VERDE, GENTE
  pinMode(pinoLEDjanela, OUTPUT); //VERMELHO, JANELA
  pinMode(pinoLEDinterno, OUTPUT); //AZUL, LATERNA INTERNA
  pinMode(pinoLEDfarol, OUTPUT); //BRANCO, FAROL
  pinMode(pinoPIR, INPUT); 
  pinMode(pinoLDRfarol, INPUT); 
  pinMode(pinoLDRinterno, INPUT);
  LIGADO = false;
  FAROL = false;
  LUZ = false;
}

void debug(int valor){
  if(valor == 1){
    Serial.print("[DEBUG]");
    Serial.print("Umidade: ");
    Serial.print(U);
    Serial.print(" | ");
    Serial.print("Temperatura: ");
    Serial.print(T);
    Serial.print(" | ");
    Serial.print("LDR farol: ");
    Serial.print(LDRfarol);
    Serial.print(" | ");
    Serial.print("LDR interno: ");
    Serial.print(LDRinterno);
    Serial.print(" | ");
    Serial.print("PIR: ");
    Serial.print(PIR);
    Serial.println(" | ");
    //Serial.println("----------------------------------------------------");
  } else {
  }
}

void sendSensor(){
  Blynk.virtualWrite(V5, U);
  Blynk.virtualWrite(V6, T);
}


BLYNK_WRITE(V0){
  int pinValue0 = param.asInt();
  if(pinValue0 == HIGH){
    DEBUG = true;
    Serial.println("DEBUG ATIVADO!!!");
  }else{
    DEBUG = false;
  }
}

BLYNK_WRITE(V4){
  int pinValue4 = param.asInt();
  if(pinValue4 == HIGH){
    FLUXO = true;
    Serial.println("FLUXO ATIVADO!!!");
  }else{
    FLUXO = false;
    Serial.println("FLUXO DESATIVADO!!!");
  }
}

BLYNK_WRITE(V1){
  int pinValue1 = param.asInt();
  if(pinValue1 == HIGH){
    LIGADO = true;
    Serial.println("Ligou o carro.");
  }else{
    LIGADO = false;
    Serial.println("Desligou o carro.");
  }
}

BLYNK_WRITE(V2){
  int pinValue2 = param.asInt();
  if(pinValue2 == HIGH){
    FAROL = true;
    Serial.println("Ligou o farol.");
  }else{
    FAROL = false;
    Serial.println("Desligou o farol.");
  }
}

BLYNK_WRITE(V3){
  int pinValue3 = param.asInt();
  if(pinValue3 == HIGH){
    LUZ = true;
    Serial.println("Ligou a luz.");
  }else{
    LUZ = false;
    Serial.println("Desligou a luz.");
  }
}


void loop(){
  Blynk.run();
  timer.run();
  LDRfarol = analogRead(pinoLDRfarol);
  LDRinterno = analogRead(pinoLDRinterno);
  PIR = digitalRead(pinoPIR);
  LER = Serial.read();
  U = dht.readHumidity();
  T = dht.readTemperature();
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

  if(LER == '3'){
    LUZ = true;
    Serial.println("Ligou a luz.");
  }
    
  if(LER == '2'){
    LUZ = false;
    Serial.println("Desligou a luz.");
  }

  if(DEBUG == true){
    debug(1);
    }else{debug(0);}
if(FLUXO == true){
  if(LIGADO == false){ 
    Serial.print("Carro está desligado. -> ");    
    if(FAROL == true){
      digitalWrite(pinoLEDfarol, HIGH);
      if(LDRfarol < 2000){
          Serial.println("Farol está ligado!!!");
          delay(1000);
          Serial.println("Desligando o farol em 5 segundos.");
          delay(5000);
          digitalWrite(pinoLEDfarol, LOW);
          FAROL = false;
          Serial.println("Farol está desligado!!!");
        }else{
        Serial.println("");
        }
      }else{
        Serial.println("");
    }
    if(LUZ == true){
      digitalWrite(pinoLEDinterno, HIGH);
      if(LDRinterno < 4000){
          Serial.println("Luz interna está ligada!!!");
          delay(1000);
          Serial.println("Desligando a luz interna em 5 segundos.");
          delay(5000);
          digitalWrite(pinoLEDinterno, LOW);
          LUZ = false;
          Serial.println("Luz interna está desligada!!!");
        }else{
        Serial.println("");
    }
      }else{
        }
    if(PIR == 1){
      Serial.print("Animal ou criança dentro do carro! | ");
      digitalWrite(pinoLEDdentro, HIGH);
      if(T > 28){
        Serial.print("Temperatura alta: ");
        Serial.print(T);
        Serial.print(" | ");
        digitalWrite(pinoLEDjanela, HIGH);
        Serial.println("Descendo as janelas.");    
          }
      }
  }else{
    Serial.print("Carro está ligado. -> ");
    digitalWrite(pinoLEDdentro, LOW);
    digitalWrite(pinoLEDinterno, LOW);
    if(FAROL == true){
      digitalWrite(pinoLEDfarol, HIGH);
      if(LDRfarol < 2000){
        Serial.print("Farol está ligado | ");
      }else{
        Serial.print("Farol está queimado | ");
      }
    }
    if(FAROL == false){
      digitalWrite(pinoLEDfarol, LOW);
      Serial.print("Farol está desligado | ");
    }
    if(LUZ == true){
      digitalWrite(pinoLEDinterno, HIGH);
      if(LDRinterno < 4000){
        Serial.println("Luz interna está ligada.");
      }else{
        Serial.println("Luz interna está está queimada.");
      }
    }
    if(FAROL == false){
      digitalWrite(pinoLEDinterno, LOW);
      Serial.println("Luz interna está desligada.");
    }
  }
}else{}

   delay(3000);
}
