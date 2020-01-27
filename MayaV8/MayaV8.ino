#include <MayaNuvem.h>  //FUNCIONANDO
#include <MayaLDR.h>    //FUNCIONANDO
#include <MayaTI.h>     //APARENTEMENTE FUNCIONANDO
#include <MayaDHT.h>    //FUNCIONANDO
#include <MayaMPU.h>    //FUNCIONANDO
#include <ArduinoJson.h>//FUNCIONANDO 
#include <MayaBLE.h>    //APARENTEMENTE FUNCIONANDO
#include <WiFi.h>  //FUNCIONANDO
#include "time.h"

String dthr;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
 
//nao esqueca de ajustar o fuso
const int   daylightOffset_sec = -3600*3;
 
void printLocalTime(){
    struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
 // dthr = (&timeinfo, "%A, %B %d %Y %H:%M:%S");
}



MayaLDR ldr(32);
MayaDHT dht(16, DHT11);
MayaMPU mpu(22,21);
MayaNuvem nuvem;

//MayaTI ti(4);
MayaBLE ble;

const int dados = JSON_OBJECT_SIZE(11);
StaticJsonDocument<dados> doc;

void setup() {
  Serial.begin(9600);
     WiFi.begin("inexoravel", "size1080p");
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
        
    }
    Serial.print("Conectado ao WIFI");
  mpu.montar();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  ldr.ativar(1);
  dht.ativar(1);
  mpu.ativar(1);
  //ti.ativar(1);
 // ble.ativar(1);

  if ( nuvem.ativar() ){
    printLocalTime();
    int dados_ldr[3] = {};
    ldr.getDados(dados_ldr);

    doc["codigo"] = 1;

    //doc["dthr_ini"] = x;
    //doc["dthr_fim"] = datahoraFim;

    doc["luz_c"]  = dados_ldr[1];
    doc["luz_s"]  = dados_ldr[2];
    doc["luz_e"]  = dados_ldr[3];

    int dados_dht[2] = {};
    dht.getDados(dados_dht);  
    
    doc["temp"] = dados_dht[1];
    doc["umid"] = dados_dht[2];

    int dados_mpu[4] = {};
    mpu.getDados(dados_mpu);
    
    doc["pos_sent"]   = dados_mpu[1];  
    doc["pos_empe"]   = dados_mpu[2];
    doc["pos_deita"]  = dados_mpu[3];
    doc["passos"]     = dados_mpu[4];

/*
    //int dados_ble[1] = {};
    //ble.getDados(dados_ble);
    //doc["cardiaco"] = dados_ble[1];
*/
    nuvem.enviar(doc);
 }

} 
