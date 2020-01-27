#include <ArduinoJson.h>
String tempo;
int quantidade;
String unix;

void JsonEnviar(){
StaticJsonDocument<200> doc;
doc["cod"]      = "A001";
doc["dt_hr"]       = tempo; //vamos tirar de qualquer forma.
doc["balanca"]     = peso;
doc["temperatura"] = degreesC; 
doc["quantidade"]  = 0.1;
//doc["umidade"]     = umi;
//doc["envio"] = "despejar"; // "despejar" para fazer funcionar lá no loop.
//doc["dt-hr-unix"] = unix;

Serial.println();
serializeJson(doc, Serial);
}

void JsonReceber(){
StaticJsonDocument<300> doc;
String input = Serial.readString();
deserializeJson(doc, input);
JsonObject obj = doc.as<JsonObject>();
String envio = obj["envio"]; //receber do rasp (receber despejar)
String Squantidade = obj["quantidade"]; //receber do rasp
//String unix = obj["dt-hr-unix"]; //recebe o tempo
quantidade = Squantidade.toInt();
Serial.println(envio);
teste = envio;
}

//toInt()
