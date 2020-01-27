#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

char ssid[] = "inexoravel"; //nome do seu WiFi
char pass[] = "size1080p";  //senha do WiFi

char auth[] = "zwLH8Wds5_mmLxfSrmNppgiwMv5bCaTs"; //autenticador do blynk
float U; 
float T; 
float U2;
//int BPM;
String BPM = "Undefined Fantastic Inex";
bool DEBUG;
DHT dht = DHT(13, DHT11);
BlynkTimer timer;

void setup(){
  Serial.begin(9600);
  Blynk.config(auth);
  Blynk.connectWiFi(ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  DEBUG = true;
}

void debug(bool DEBUG){
  if(DEBUG == true){
    Serial.print("[DEBUG]");
    Serial.print("Umidade: ");
    Serial.print(U);
    Serial.print(" | ");
    Serial.print("Temperatura: ");
    Serial.print(T);
    Serial.print(" | ");
    Serial.print("Batimento cardíaco: ");
    Serial.print(BPM);
    Serial.print("Umidade: ");
    Serial.print(U2);
    Serial.print(" | ");
    Serial.println("----------------------------------------------------");
  } else {
  }
}

void sendSensor(){
  Blynk.virtualWrite(V0, U);
  Blynk.virtualWrite(V1, T);
  Blynk.virtualWrite(V2, BPM);
}


BLYNK_WRITE(V9){
  int pinValue0 = param.asInt();
  if(pinValue0 == HIGH){
    DEBUG = true;
    Serial.println("DEBUG ATIVADO!!!");
  }else{
    DEBUG = false;
  }
}


void loop(){
  bool conectado = Blynk.connect();
  if(conectado == false){
  Blynk.connect();
  }else{
    Blynk.run();
  }
  timer.run();
  U = dht.readHumidity();
  T = dht.readTemperature();
  U2 = U + 5;
    debug(true);
}
