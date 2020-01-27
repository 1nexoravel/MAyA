#include "DHT.h" 
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Wire.h>
#include <HTTPClient.h>
#include "BLEDevice.h"
#include <OneWire.h> //INCLUSÃO DE BIBLIOTECA
#include <DallasTemperature.h> //INCLUSÃO DE BIBLIOTECA
#include <ArduinoJson.h>  // biblioteca JSON para sistemas embarcados
#define dht_dpin 13// G13
#define DHTTYPE DHT11 // DHT 11
#define DS18B20 14 // G14
DHT dht(dht_dpin, DHTTYPE); 
//Sim800L GSM(RX, TX);
OneWire ourWire(DS18B20); //CONFIGURA UMA INSTÂNCIA ONEWIRE PARA SE COMUNICAR COM O SENSOR
DallasTemperature sensors(&ourWire); //BIBLIOTECA DallasTemperature UTILIZA A OneWire
WiFiMulti WiFiMulti;



/*  Declaração das Variáveis globais  */
//const char* ssid     = "inexoravel";
//const char* password = "size1080p";

const int azul = 23;
const int verde = 19;
const int vermelho = 5;

static  BLEUUID serviceUUID(BLEUUID((uint16_t)0x180D));
static  BLEUUID    charUUID(BLEUUID((uint16_t)0x2A37));
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static boolean notification = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
int bpmx = 0;
int pas = 0;
float cal = 0.08;
float hvar = 0;
float tvar = 0;
float temp_inte;
int min_empe    = 0;
int min_deitado = 0;
int min_sentado = 0;
int min_claro   = 0;
int min_sombra  = 0;
int min_escuro  = 0;
int luminosidade = 0;   
String barra = "/";
String espaco = " ";
String doispontos = ":";
const int MPU_ADDR =      0x68; // definição do endereço do sensor MPU6050 (0x68)
const int WHO_AM_I =      0x75; // registro de identificação do dispositivo
const int PWR_MGMT_1 =    0x6B; // registro de configuração do gerenciamento de energia
const int GYRO_CONFIG =   0x1B; // registro de configuração do giroscópio
const int ACCEL_CONFIG =  0x1C; // registro de configuração do acelerômetro
const int ACCEL_XOUT =    0x3B; // registro de leitura do eixo X do acelerômetro
const int sda_pin = 21; // definição do pino I2C SDA
const int scl_pin = 22; // definição do pino I2C SCL
const int analogInPin = 32;   //G32, 
const int bat = 34; //G34
int16_t AcX, AcY, AcZ; 
int day,month,year,minute,second,hour;
const uint32_t debounceTime = 1000;  // 5 mSec, enough for most switches
const uint8_t switchPin     = 12;  // definição do pino para o botão toggle
const bool switchOn  = false;     // using INPUT_PULLUP
const bool switchOff = true;
bool lastState   = switchOff;
bool newState    = switchOff;
bool toggleState = false;

// Tempo de Coleta e Envio da : BLE Batimento Cardiáco.
unsigned long periodo_bat = 6000;  // 20 mintutos = 1200000
unsigned long time_bat   = 0;  

// Tempo de Coleta e Envio da : BLE Batimento Cardiáco.
unsigned long periodo_ble = 6000;  // 20 mintutos = 1200000
unsigned long time_ble   = 0;   

// Tempo de Coleta e Envio da : Temperatura interna
unsigned long periodo_temp = 6000;  // 20 mintutos = 1200000
unsigned long time_temp    = 0;   

// Tempo de Coleta e Envio da : Temperatura e Umidade
unsigned long periodo_dht = 6000;  // 20 mintutos = 1200000
unsigned long time_dht    = 0;       

// Tempo de Coleta e Envio da : Luminosidade
unsigned long periodo_lumi = 6000;  // 20 mintutos = 1200000
unsigned long time_lumi   = 0;       

// Tempo de Coleta e Envio da : Acelerometro e Giroscópio
unsigned long periodo_mpu = 6000; // 1 minuto = 60000
unsigned long time_mpu    = 0;

// Tempo de Coleta e Envio da : Acelerometro e Giroscópio
unsigned long periodo_pod = 1000; // 1 minuto = 60000
unsigned long time_pod    = 0;

// Tempo de Coleta e Envio da : Latitude e Longitude
unsigned long periodo_geo = 6000; // 2 minutos = 120000
unsigned long time_geo    = 0;

// Tempo de envio dos dados gerais para o servidor
unsigned long periodo_json = 30000; // 1 hr = 3600000
unsigned long time_json    = 0;

void initI2C() 
{
  //Serial.println("---inside initI2C");
  Wire.begin(21, 22); //400000
  
}
 
/*
 * função que escreve um dado valor em um dado registro
 */
void writeRegMPU(int reg, int val)      //aceita um registro e um valor como parâmetro
{
  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.write(val);                      // escreve o valor no registro
  Wire.endTransmission(true);           // termina a transmissão
}
 
/*
 * função que lê de um dado registro
 */
uint8_t readRegMPU(uint8_t reg)        // aceita um registro como parâmetro
{
  uint8_t data;
  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.endTransmission(false);          // termina transmissão mas continua com I2C aberto (envia STOP e START)
  Wire.requestFrom(MPU_ADDR, 1);        // configura para receber 1 byte do registro escolhido acima
  data = Wire.read();                   // lê o byte e guarda em 'data'
  return data;                          //retorna 'data'
}


void findMPU(int mpu_addr)
{
  Wire.beginTransmission(MPU_ADDR);
  int data = Wire.endTransmission(true);
 
  if(data == 0)
  {
    Serial.print("Dispositivo encontrado no endereço: 0x");
    Serial.println(MPU_ADDR, HEX);
  }
  else
  {
    Serial.println("Dispositivo não encontrado!");
  }
}

void checkMPU(int mpu_addr)
{
  findMPU(MPU_ADDR);
     
  int data = readRegMPU(WHO_AM_I); // Register 117 – Who Am I - 0x75
   
  if(data == 104) 
  {
    Serial.println("MPU6050 Dispositivo respondeu OK! (104)");
 
    data = readRegMPU(PWR_MGMT_1); // Register 107 – Power Management 1-0x6B
 
    if(data == 64) Serial.println("MPU6050 em modo SLEEP! (64)");
    else Serial.println("MPU6050 em modo ACTIVE!"); 
  }
  else Serial.println("Verifique dispositivo - MPU6050 NÃO disponível!");
}

void initMPU()
{
  setSleepOff();
  setGyroScale();
  setAccelScale();
}

void setSleepOff()
{
  writeRegMPU(PWR_MGMT_1, 0); // escreve 0 no registro de gerenciamento de energia(0x68), colocando o sensor em o modo ACTIVE
}

void setGyroScale()
{
  writeRegMPU(GYRO_CONFIG, 0);
}

void setAccelScale()
{
  writeRegMPU(ACCEL_CONFIG, 0);
}

void readRawMPU()
{  
  Wire.beginTransmission(MPU_ADDR);       // inicia comunicação com endereço do MPU6050
  Wire.write(ACCEL_XOUT);                       // envia o registro com o qual se deseja trabalhar, começando com registro 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);            // termina transmissão mas continua com I2C aberto (envia STOP e START)
  Wire.requestFrom(MPU_ADDR, 14);         // configura para receber 14 bytes começando do registro escolhido acima (0x3B)
 
  AcX = Wire.read() << 8;                 // lê primeiro o byte mais significativo
  AcX |= Wire.read();                     // depois lê o bit menos significativo
  AcY = Wire.read() << 8;
  AcY |= Wire.read();
  AcZ = Wire.read() << 8;
  AcZ |= Wire.read(); 
}

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    if(length == 2){
      bpmx = pData[1];
    }
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      digitalWrite(azul, LOW);
      return false;
    }
    Serial.println(" - Found our service");
    


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      digitalWrite(azul, LOW);
      return false;
     
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
      
    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void BLE_SETUP() {
  //Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.

void botao(){
  newState = digitalRead( switchPin );

  if( lastState != newState ) // state changed
  {
    delay( debounceTime );
    lastState = newState;
    
    // push on, push off
    if( newState == switchOn && toggleState == false )
    {
      toggleState = true;
      Serial.println( F ( "Switched ON" ) );
      
    }
    else if( newState == switchOn && toggleState == true )
    {
      toggleState = false;
      Serial.println( F ( "Switched OFF" ) );
    }
  }  
} 



// Criação do objeto para montar o JSON
StaticJsonBuffer<450> jsonBuffer;
JsonObject& object    = jsonBuffer.createObject();

// Genericos
JsonObject& cod       = object.createNestedObject("cod");

JsonObject& dt_hr_ini = object.createNestedObject("dt_hr_ini");
JsonObject& dt_hr_fim = object.createNestedObject("dt_hr_fim");
JsonObject& rotina    = object.createNestedObject("rotina");

//Atributos para BLE, BPM
JsonObject& bpm = object.createNestedObject("bpm");

//Atributos para BLE, BPM
JsonObject& passos = object.createNestedObject("passos");
JsonObject& calorias = object.createNestedObject("calorias");

//Atributos para Temperatura Interna
JsonObject& temp_interna = object.createNestedObject("temp_interna");

// Atributos para o DHT
JsonObject& temp = object.createNestedObject("temp");
JsonObject& umi  = object.createNestedObject("umi");

// Atributos para o LUMI
JsonObject& lumi  = object.createNestedObject("lumi");
JsonObject& tempo_claro  = lumi.createNestedObject("tempo_claro");
JsonObject& tempo_sombra  = lumi.createNestedObject("tempo_sombra");
JsonObject& tempo_escuro  = lumi.createNestedObject("tempo_escuro");

//Atributos para o MPU
JsonObject& pos  = object.createNestedObject("status");
JsonObject& tempo_empe  = pos.createNestedObject("tempo_empe");
JsonObject& tempo_deitado  = pos.createNestedObject("tempo_deitado");
JsonObject& tempo_sentado  = pos.createNestedObject("tempo_sentado");

//Atributos para Geolocalização
JsonObject& lat = object.createNestedObject("lat");
JsonObject& lon = object.createNestedObject("lon");




int enviardados(String JSONmessageBuffer){
  HTTPClient http;

  http.begin("http://3.88.193.51/joao/send-post.php");
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(JSONmessageBuffer);

  http.end();
  return httpCode;
}

void coletaBAT(){ // AINDA N FOI TESTADO!
  if(millis() >  time_bat + periodo_bat){
    time_bat = millis();
    if (bat < 3500){
      Serial.print("bateria fraca");
      digitalWrite(vermelho, LOW);
    }
  }
}

void coletaBLE() {
 if(millis() >  time_ble + periodo_ble){
    time_ble = millis();
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
        if (notification == false) {
      Serial.println(F("Turning Notifocation On"));
      const uint8_t onPacket[] = {0x1, 0x0};
      pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)onPacket, 2, true);
      notification = true;
    }
  }
    Serial.println("[CARDIACO]");
    Serial.print("Batimentos (por minuto):");
    Serial.println(bpmx);
    Serial.println(" -------------- ");
 }
}

void coletaINTERNA(){
  if(millis() >  time_temp + periodo_temp){
    time_temp = millis();
    sensors.requestTemperatures();
    temp_inte = sensors.getTempCByIndex(0);
    Serial.println("[INTERNO]");
    Serial.print("Termometro: ");
    Serial.println(temp_inte);
    Serial.println(" -------------- ");
  }
}

void coletaDHT(){ 
  if(millis() >  time_dht + periodo_dht){
    time_dht = millis(); 
    //hvar = hvar + dht.readHumidity();
   // tvar = tvar + dht.readTemperature();
    //hvar = dht.readHumidity();
    //tvar = dht.readTemperature();
    Serial.println("[AMBIENTE]");
    Serial.print("Temperatura: ");
    Serial.println(hvar);
    Serial.print("Umidade: ");
    Serial.println(tvar);
    Serial.println(" -------------- ");
  }
}

void coletaLUMI(){
  luminosidade = analogRead(analogInPin);
    if(millis() >  time_lumi + periodo_lumi){
      time_lumi = millis(); 
       if(luminosidade > 500){
        min_claro++;
//        Serial.print("min_claro :");
//        Serial.println(min_claro);

        }
        if(luminosidade <= 500 && luminosidade > 100){
        min_sombra++;
//        Serial.print("min_sombra :");
//        Serial.println(min_sombra);
        }
        if(luminosidade <= 100){
        min_escuro++;
//        Serial.print("min_escuro :");
//        Serial.println(min_escuro);
        }

        Serial.println("[LUMINOSIDADE - POR MINUTO]");
        Serial.print("Claro: ");
        Serial.println(min_claro);
        Serial.print("Sombra: ");
        Serial.println(min_sombra);
        Serial.print("Escuro: ");
        Serial.println(min_escuro);
        Serial.println(" -------------- ");
      }
  }
void coletaMPU(){
  if(millis() >  time_mpu + periodo_mpu){
    time_mpu = millis();  
      readRawMPU();
 //     Serial.print("AcX = "); Serial.print(AcX);
  //    Serial.print(" | AcY = "); Serial.print(AcY);
  //    Serial.print(" | AcZ = "); Serial.print(AcZ);
  //    Serial.println(); 
    //if(AcX > 11000 && AcX < 17000 && AcY > -4000 && AcY < 4000 && AcZ > -16000 && AcZ < -10000){
    if(AcX > -7000 && AcX < 7000 && AcY > 5000 && AcY < 13000 && AcZ > -16000 && AcZ < -10000){
      min_empe++;
      
     // Serial.print("Minutos em pé: ");
     // Serial.println(min_empe);
      }
    //if(AcX > -7000 && AcX < 2000 && AcY > -2000 && AcY < 3000 && AcZ > -18000 && AcZ < -14000){
    else if(AcX > -2000 && AcX < 3000 && AcY > -7000 && AcY < 2000 && AcZ > -18000 && AcZ < -14000){
      min_deitado++;
      
     // Serial.print("Minutos deitado: ");
     // Serial.println(min_deitado);
      }
    // if(AcX > -1000 && AcX < 3000 && ((AcY > 10000 && AcY < 18000)||(AcY > -18000 && AcY < -10000)) && AcZ > -10000 && AcZ < 3000){
    else if(((AcX > 10000 && AcX < 18000)||(AcX > -18000 && AcX < -10000)) && AcY > -1000 && AcY < 3000 && AcZ > -10000 && AcZ < 3000){
      min_deitado++;
      
     // Serial.print("min_deitado :");
     // Serial.println(min_deitado);
      }                                                                         
   // if(AcX > 12000 && AcX < 18000 && AcY > -4000 && AcY < 4000 && AcZ > -12000 && AcZ < 1000){
    else if(AcX > -4000 && AcX < 4000 && AcY > 12000 && AcY < 18000 && AcZ > -12000 && AcZ < 1000){
      min_sentado++;
      
     // Serial.println("min_sentado :");
     // Serial.println(min_sentado);
      }
      else{
        min_deitado++;
        }
  
    Serial.println("[POSICAO - POR MINUTO]");
    Serial.print("Em Pé: ");
    Serial.println(min_empe);
    Serial.print("Deitado: ");
    Serial.println(min_deitado);
    Serial.print("Sentado: ");
    Serial.println(min_sentado);
    Serial.println(" -------------- ");
   } 
}

void coletaPEDO(){
  if(millis() >  time_pod + periodo_pod){
    time_pod = millis();  
      if ((AcX < -6000 || AcZ < -6000 || AcX > 6000 || AcZ > 6000) && (AcY > 12000)) {
          pas++;

          Serial.println("[PASSOS]");
          Serial.print("Quantidade: ");
          Serial.println(pas);
          Serial.println(" -------------- ");
          Serial.print("[CALORIAS]");
          Serial.println(pas*cal);
//      if (aaWorld.x < -1000 || aaWorld.y < -1000 || aaWorld.x > 1000 || aaWorld.y > 1000) {
//                    passos++;
      }
  }
}

void coletaGEO(){ 
  if(millis() >  time_geo + periodo_geo){
    time_geo = millis(); 
    //GSM.RTCtime(&day,&month,&year,&hour,&minute,&second);
  }
}

String monta_JSON(String tipo){
  object["cod"]         = "C001";
  object["dt_hr_ini"]   = day+barra+month+barra+year+espaco+hour+doispontos+minute+doispontos+second;
  object["dt_hr_fim"]   = day+barra+month+barra+year+espaco+hour+doispontos+(minute+10)+doispontos+second;
  if (tipo.equals("PEDO")){
      object["rotina"] = "PEDO";
      object["passos"] = pas;
      object["calorias"] = pas*cal;
  }
  
  if (tipo.equals("BPM")){
    object["rotina"] = "BPM";
    object["bpm"] = bpmx;
  }
  
  if (tipo.equals("TEMP")){
    object["rotina"] = "TEMP";
    object["temp_interna"]  = temp_inte;
  }   
  
  if (tipo.equals("LUMI")){
    object["rotina"] = "LUMI";
    lumi["tempo_claro"]  = min_claro;
    lumi["tempo_sombra"] = min_sombra;
    lumi["tempo_escuro"] = min_escuro;
    min_claro   = 0;
    min_sombra  = 0;
    min_escuro  = 0;
  }    
  if (tipo.equals("DHT")) {   
      object["rotina"]    = "DHT";
      object["temp"]      = tvar/5;
      object["umi"]       = hvar/5;
      tvar = 0;
      hvar = 0;
     
  }
  if (tipo.equals("MPU")) {
    object["rotina"] = "MPU";
    pos["tempo_sentado"] = min_sentado;
    pos["tempo_deitado"] = min_deitado;
    pos["tempo_empe"]   = min_empe;   
    min_sentado = 0;
    min_empe    = 0;
    min_deitado = 0;

  } 
  if (tipo.equals("GEO")){
    object["rotina"] = "GEO";
    //object["lat"] = GSM.getLatitude();
    //object["lon"] = GSM.getLongitude();
  }
 

  char JSONmessageBuffer[300];
  object.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  //Serial.println(JSONmessageBuffer);
  return JSONmessageBuffer;

}

bool checaenvio(){
 if(millis() >  time_json + periodo_json){
    time_json = millis(); 
    Serial.println("Inicio do envio para Servidor");
    String v = monta_JSON("BPM");
    Serial.println(v);
//    String x = monta_JSON("TEMP");
//    Serial.println(x);
//    String y = monta_JSON("LUMI");
//    Serial.println(y);
//    String z = monta_JSON("DHT");
//    Serial.println(z);
//    String w = monta_JSON("MPU");
//    Serial.println(w);

    //int rsDHT = enviardados(x); //banco de dados
    //String rsMPU = enviardados( monta_JSON("MPU") );
    //String rsGEO = enviardados( monta_JSON("GEO") );
    //Serial.println(rsDHT); //banco de dados
    //Serial.println(rsMPU);
    //Serial.println(rsGEO);
    return true;       
 }
 return false;
}

void setup() {
  pinMode(azul, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(vermelho, OUTPUT);
  digitalWrite(azul, HIGH);
  digitalWrite(verde, HIGH);
  digitalWrite(vermelho, HIGH);
  dht.begin(); 
  sensors.begin();
  Serial.begin(9600);
  BLE_SETUP();
  //GSM.begin(4800);  

      // We start by connecting to a WiFi network
    WiFiMulti.addAP("martins", "martins0123");

    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
        digitalWrite(verde, LOW); //LED VERDE
    }
    digitalWrite(verde, HIGH);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    delay(500);

  initI2C();
  initMPU();
  checkMPU(MPU_ADDR);
  pinMode ( switchPin, INPUT_PULLUP );
  
 // Serial.println("WI-FI conectado com sucesso");
}

  
void loop() {
  coletaLUMI();
  coletaDHT();
  coletaMPU();
  coletaPEDO();
 // botao();
  coletaBLE();
 // coletaINTERNA();
  //coletaBAT(); //falta usar o pino alimentado pela bateria
  if (toggleState == true){
    //checaenvio();
    Serial.println("Informações enviadas para o servidor!");
  }


  //coletaGEO();
}


 
