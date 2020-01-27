//#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include "BLEDevice.h"

char ssid[] = "inexoravel"; //nome do seu WiFi
char pass[] = "size1080p";  //senha do WiFi

char auth[] = "zwLH8Wds5_mmLxfSrmNppgiwMv5bCaTs"; //autenticador do blynk

int lastConnectionAttempt = millis();
int connectionDelay = 5000; // try to reconnect every 5 seconds

float U; 
float T; 
int BPM = 100;
//String BPM = "UFI";
bool DEBUG;
bool conectado;
DHT dht = DHT(13, DHT11);
BlynkTimer timer;

static  BLEUUID serviceUUID(BLEUUID((uint16_t)0x180D));
static  BLEUUID    charUUID(BLEUUID((uint16_t)0x2A37));

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static boolean notification = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    if(length == 2){
      BPM = (pData[1]);   
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
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
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
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
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
    Serial.print(" | ");
    Serial.println(" ");
  } else {
  }
}

void sendSensor(){
  Blynk.virtualWrite(V0, T);
  Blynk.virtualWrite(V1, U);
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
void setup(){
  Serial.begin(9600);
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
  
  WiFi.begin((char*)ssid, (char*)pass);
  Blynk.config(auth);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  DEBUG = true;  
}

void loop(){
  if (WiFi.status() != WL_CONNECTED){
    if (millis() - lastConnectionAttempt >= connectionDelay){
      lastConnectionAttempt = millis();
      if (pass && strlen(pass)){
        Serial.println("Tentando reconectar...");
        WiFi.begin((char*)ssid, (char*)pass);
      }else{
        Serial.println("Tentando reconectar...");
        WiFi.begin((char*)ssid);}}
  }else{Blynk.run();}
  timer.run();
  U = dht.readHumidity();
  T = dht.readTemperature();
    debug(DEBUG);
  delay(1000);

  if(doConnect == true){
    if (connectToServer()){
      Serial.println("We are now connected to the BLE Server.");
    }else{
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
}
