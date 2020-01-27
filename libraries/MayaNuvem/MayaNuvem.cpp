#include "MayaNuvem.h"
#include <ArduinoJson.h>  
#include <HTTPClient.h>
 MayaNuvem::MayaNuvem(){
    periodo_ = 10000; //20 mintutos = 1200000
    tempo_   = 0;
    url_     = "http://3.88.193.51/joao/send-post.php";
  }

  void MayaNuvem::setPeriodo(int periodo){
    periodo_ = periodo;
  }

  void MayaNuvem::setTempo(int tempo){
    tempo_ = tempo;
  }


  bool MayaNuvem::ativar(){
    if( millis() >  tempo_ + periodo_ ){
      tempo_ = millis();
      return true;
    }
    return false;
  }

  bool MayaNuvem::enviar(const JsonDocument& doc){
	  Serial.println("0");
	  	String output;
		Serial.println("1");
    serializeJson(doc, output);
    Serial.println(output);
    Serial.println("");
	
		Serial.println("2");
    HTTPClient http;
		Serial.println("3");
    http.begin(url_);
		Serial.println("4");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(output);
		Serial.println("6");
    http.end();
		Serial.println("7");
    Serial.println("***************");    
    Serial.println(httpCode); 
    Serial.println("***************");    

    return true;
  }
