#include <MayaNuvem.h>
#include <MayaLDR.h>
#include <MayaTI.h>
#include <MayaDHT.h>
#include <MayaMPU.h>
#include <ArduinoJson.h>
#include <MayaBLE.h>

MayaLDR ldr(32);
MayaTI ti(4);
MayaDHT dht(16, DHT11);
MayaMPU mpu(22,11);
MayaNuvem nuvem;
MayaBLE ble;

const int dados = JSON_OBJECT_SIZE(11);
StaticJsonDocument<dados> doc;

void setup() {
  Serial.begin(9600);
  ti.montar();
}

void loop() {
  ldr.ativar(1);
  dht.ativar(1);
  ti.ativar(1);
  mpu.ativar(1);
  ble.ativar(1);

  if ( nuvem.ativar() ){
    
    int dados_ldr[3] = {};
    ldr.getDados(dados_ldr);
 
    doc["codigo"] = 1;
    
    doc["luz_e"]  = dados_ldr[1];
    doc["luz_s"]  = dados_ldr[2];
    doc["luz_c"]  = dados_ldr[3];

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

    int dados_ble[1] = {};
    ble.getDados(dados_ble);
    doc["cardiaco"] = dados_ble[1];

   
    nuvem.enviar(doc);
  }

} 
