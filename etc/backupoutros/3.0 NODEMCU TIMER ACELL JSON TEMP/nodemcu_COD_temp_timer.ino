/* ESP8266 e Banco de Dados - Requisições HTTP
 * 2016 por José Cintra
 * www.josecintra.com/blog
 */ 
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <Wire.h>         // biblioteca de comunicação I2C
#include <ArduinoJson.h>  // biblioteca JSON para sistemas embarcados
#define DHTPIN D3 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

 
// Site remoto - Coloque aqui os dados do site que vai receber a requisição GET
//const char http_site[] = "http://3.88.193.51/joao";
//const int http_port = 80;
 
// Variáveis globais
//WiFiClient client;
//IPAddress server(3,88,193,51); //Endereço IP do servidor - http_site
float hvar = 0;
float tvar = 0;
String co = "C001";


const int MPU_ADDR =      0x68; // definição do endereço do sensor MPU6050 (0x68)
const int WHO_AM_I =      0x75; // registro de identificação do dispositivo
const int PWR_MGMT_1 =    0x6B; // registro de configuração do gerenciamento de energia
const int GYRO_CONFIG =   0x1B; // registro de configuração do giroscópio
const int ACCEL_CONFIG =  0x1C; // registro de configuração do acelerômetro
const int ACCEL_XOUT =    0x3B; // registro de leitura do eixo X do acelerômetro
 
const int sda_pin = D5; // definição do pino I2C SDA
const int scl_pin = D6; // definição do pino I2C SCL
 
bool led_state = false;
 
// variáveis para armazenar os dados "crus" do acelerômetro
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; 

//  StaticJsonBuffer<350> jsonBuffer;
//  JsonObject& object = jsonBuffer.createObject();
//JsonObject& cod = object.createNestedObject("cod");
//   
////JsonObject& accel = data.createNestedObject("accel");
//JsonObject& temp = object.createNestedObject("temp");
//JsonObject& umi = object.createNestedObject("umi");
//JsonObject& dt_hr_ini = object.createNestedObject("dt_hr_ini");
//JsonObject& dt_hr_fim = object.createNestedObject("dt_hr_fim");
//JsonObject& gyro = data.createNestedObject("gyro");
//JsonObject& dht = data.createNestedObject("dht");
   
//JsonObject& accelX = accel.createNestedObject("accelX");
//JsonObject& accelY = accel.createNestedObject("accelY");
//JsonObject& accelZ = accel.createNestedObject("accelZ");
 
//JsonObject& gyroX = gyro.createNestedObject("gyroX");
//JsonObject& gyroY = gyro.createNestedObject("gyroY");
//JsonObject& gyroZ = gyro.createNestedObject("gyroZ");

 
/*
 * função que configura a I2C com os pinos desejados 
 * sda_pin -> D5
 * scl_pin -> D6
 */
//void initI2C() 
//{
//  //Serial.println("---inside initI2C");
//  Wire.begin(sda_pin, scl_pin);
//  
//}
// 
///*
// * função que escreve um dado valor em um dado registro
// */
//void writeRegMPU(int reg, int val)      //aceita um registro e um valor como parâmetro
//{
//  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
//  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
//  Wire.write(val);                      // escreve o valor no registro
//  Wire.endTransmission(true);           // termina a transmissão
//}
// 
///*
// * função que lê de um dado registro
// */
//uint8_t readRegMPU(uint8_t reg)        // aceita um registro como parâmetro
//{
//  uint8_t data;
//  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
//  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
//  Wire.endTransmission(false);          // termina transmissão mas continua com I2C aberto (envia STOP e START)
//  Wire.requestFrom(MPU_ADDR, 1);        // configura para receber 1 byte do registro escolhido acima
//  data = Wire.read();                   // lê o byte e guarda em 'data'
//  return data;                          //retorna 'data'
//}
//
//
//void findMPU(int mpu_addr)
//{
//  Wire.beginTransmission(MPU_ADDR);
//  int data = Wire.endTransmission(true);
// 
//  if(data == 0)
//  {
//    Serial.print("Dispositivo encontrado no endereço: 0x");
//    Serial.println(MPU_ADDR, HEX);
//  }
//  else
//  {
//    Serial.println("Dispositivo não encontrado!");
//  }
//}
//
//void checkMPU(int mpu_addr)
//{
//  findMPU(MPU_ADDR);
//     
//  int data = readRegMPU(WHO_AM_I); // Register 117 – Who Am I - 0x75
//   
//  if(data == 104) 
//  {
//    Serial.println("MPU6050 Dispositivo respondeu OK! (104)");
// 
//    data = readRegMPU(PWR_MGMT_1); // Register 107 – Power Management 1-0x6B
// 
//    if(data == 64) Serial.println("MPU6050 em modo SLEEP! (64)");
//    else Serial.println("MPU6050 em modo ACTIVE!"); 
//  }
//  else Serial.println("Verifique dispositivo - MPU6050 NÃO disponível!");
//}
//
//void initMPU()
//{
//  setSleepOff();
//  setGyroScale();
//  setAccelScale();
//}
//
//void setSleepOff()
//{
//  writeRegMPU(PWR_MGMT_1, 0); // escreve 0 no registro de gerenciamento de energia(0x68), colocando o sensor em o modo ACTIVE
//}
//
//void setGyroScale()
//{
//  writeRegMPU(GYRO_CONFIG, 0);
//}
//
//void setAccelScale()
//{
//  writeRegMPU(ACCEL_CONFIG, 0);
//}

//void readRawMPU()
//{  
//  Wire.beginTransmission(MPU_ADDR);       // inicia comunicação com endereço do MPU6050
//  Wire.write(ACCEL_XOUT);                       // envia o registro com o qual se deseja trabalhar, começando com registro 0x3B (ACCEL_XOUT_H)
//  Wire.endTransmission(false);            // termina transmissão mas continua com I2C aberto (envia STOP e START)
//  Wire.requestFrom(MPU_ADDR, 14);         // configura para receber 14 bytes começando do registro escolhido acima (0x3B)
// 
//  AcX = Wire.read() << 8;                 // lê primeiro o byte mais significativo
//  AcX |= Wire.read();                     // depois lê o bit menos significativo
//  AcY = Wire.read() << 8;
//  AcY |= Wire.read();
//  AcZ = Wire.read() << 8;
//  AcZ |= Wire.read();
// 
//  Tmp = Wire.read() << 8;
//  Tmp |= Wire.read();
// 
//  GyX = Wire.read() << 8;
//  GyX |= Wire.read();
//  GyY = Wire.read() << 8;
//  GyY |= Wire.read();
//  GyZ = Wire.read() << 8;
//  GyZ |= Wire.read(); 

 
//  Serial.print("AcX = "); Serial.print(AcX);
//  Serial.print(" | AcY = "); Serial.print(AcY);
//  Serial.print(" | AcZ = "); Serial.print(AcZ);
//  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);
//  Serial.print(" | GyX = "); Serial.print(GyX);
//  Serial.print(" | GyY = "); Serial.print(GyY);
//  Serial.print(" | GyZ = "); Serial.println(GyZ);
 
//  led_state = !led_state;
//  digitalWrite(LED_BUILTIN, led_state);         // pisca LED do NodeMCU a cada leitura do sensor
//  delay(50);                                        
//}

//  void populateJSON()
//  {
//    object["cod"] = co;
//    object["temp"] = t;
//    object["umi"] = h;
//    object["dt_hr_ini"] = "2019-08-01 10:00:00";
//    object["dt_hr_fim"] = "2019-08-01 10:09:00";
//   
//    //gyro["gyroX"] = GyX;
//    //gyro["gyroY"] = GyY;
//    //gyro["gyroZ"] = GyZ;
//    //accel["accelX"] = AcX;
//    //accel["accelY"] = AcY;
//    //accel["accelZ"] = AcZ;
//  }


 StaticJsonBuffer<350> jsonBuffer;
JsonObject& object = jsonBuffer.createObject();
JsonObject& cod = object.createNestedObject("cod");
JsonObject& temp = object.createNestedObject("temp");
JsonObject& umi = object.createNestedObject("umi");
JsonObject& dt_hr_ini = object.createNestedObject("dt_hr_ini");
JsonObject& dt_hr_fim = object.createNestedObject("dt_hr_fim");
 

String enviardados(String JSONmessageBuffer){
 HTTPClient http;    //Declare object of class HTTPClient
 
            http.begin("http://3.88.193.51/joao/send-temp-post.php");      //Specify request destination
            http.addHeader("Content-Type", "application/json");  //Specify content-type header
         
            int httpCode = http.POST(JSONmessageBuffer);   //Send the request
            String payload = http.getString();                                        //Get the response payload
         
//            Serial.println(httpCode);   //Print HTTP return code
//            Serial.println(payload);    //Print request response payload
         
            http.end();  //Close connection
            return payload;
}



//unsigned long time_now = millis();
unsigned long periodo = 1000;
unsigned long periodo_dht = 6000;
unsigned long time_dht = 0;
unsigned long periodo_json = 30000;
unsigned long time_json = 0;
//timestamp

//void mil(){
// // if(millis() > time_now + period){
// // time_now = millis(); 
//  if(millis() > period){
//  period = period + period; 
//        hvar = hvar + dht.readHumidity();
//      tvar = tvar + dht.readTemperature();
//      Serial.println(hvar);
//      Serial.println(tvar);
//  }
//}

//void mil(){
//  if(millis() >  period){
//  period = millis() + period; 
//      hvar = hvar + dht.readHumidity();
//      tvar = tvar + dht.readTemperature();
//      Serial.println(hvar);
//      Serial.println(tvar);
//  }
//}

void coletaDHT(){ 
      if(millis() >  time_dht + periodo_dht){
      time_dht = millis(); 
      hvar = hvar + dht.readHumidity();
      tvar = tvar + dht.readTemperature();
      Serial.println(hvar);
      Serial.println(tvar);
      
  }
}
  


  
String monta_JSON(String tipo){
  
  object["cod"] = co;
  
  if (tipo.equals("DHT")) {
     
      object["temp"] = tvar/10;
      object["umi"] = hvar/10;
      object["dt_hr_ini"] = "2019-08-01 10:00:00";
      object["dt_hr_fim"] = "2019-08-01 10:09:00";
      tvar = 0;
      hvar = 0;
     
  }
  if (tipo.equals("X")) {
      object["cod"] = co;
      object["temp"] = tvar/10;
      object["umi"] = hvar/10;
      object["dt_hr_ini"] = "2019-08-01 10:00:00";
      object["dt_hr_fim"] = "2019-08-01 10:09:00";
      
  }
  
            char JSONmessageBuffer[300];
            object.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
            Serial.println("monta json: ");
            Serial.println(JSONmessageBuffer);
            return JSONmessageBuffer;
  }












void setup() {
  //delay(3000); //Aguarda 30 segundos 
  Serial.begin(9600);
  dht.begin();
//  Serial.println("NodeMCU - Gravando dadios no BD via GET");
//  Serial.println("Aguardando conexão");
//  
//  // Tenta conexão com Wi-fi                         
  WiFi.begin("inexoravel", "size1080p");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
  Serial.print("\nWI-FI conectado com sucesso: ");

//Serial.println("nIniciando configuração do MPU6050n");
//  initI2C();
//  initMPU();
//  checkMPU(MPU_ADDR);
// 
//  Serial.println("Enviando localização ao servidorn");
//  //makePOSTlocation();
// 
//  Serial.println("nConfiguração finalizada, iniciando loopn");
// 
}
 
void loop() {
//  delay(6000); //delay entre as leituras
//  h=dht.readHumidity();
//  t=dht.readTemperature();
//  Serial.println(t);
//  Serial.println(h); 

coletaDHT();
 if(millis() >  time_json + periodo_json){
      time_json = millis(); 
    //    StaticJsonBuffer<350> jsonBufferDHT;
     //   JsonObject objectDHT = jsonBufferDHT.createObject();
            String json = monta_JSON("DHT");
            String retorno = enviardados(json);
            Serial.println(retorno);
            
          

            
           
           
         
 }
}


 
