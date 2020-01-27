//#include "DHT.h"
//#define DHTPIN A2 // pino que estamos conectado
//#define DHTTYPE DHT11 // DHT 11
//DHT dht(DHTPIN, DHTTYPE);
//float umi;
//float temp;
//
//void tu(){
// umi = dht.readHumidity();
// temp = dht.readTemperature();
//}
//
//void tusetup(){
//  dht.begin();
//}

const int temperaturePin = A2;


float getVoltage(int pin){
  return (analogRead(pin) * 0.01302); //calibração nova!! 
}

void temperatura(){
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
}
