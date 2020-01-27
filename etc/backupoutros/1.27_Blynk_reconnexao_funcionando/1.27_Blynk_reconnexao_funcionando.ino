/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
int ReCnctFlag;  // Reconnection Flag
int ReCnctCount = 0;  // Reconnection counter

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "zwLH8Wds5_mmLxfSrmNppgiwMv5bCaTs";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "inexoravel";
char pass[] = "size1080p";
bool x;
BlynkTimer timer;

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  x = true;
  //timer.setInterval(15000L, reconnectBlynk);
}
BLYNK_CONNECTED() {
  Serial.println("Cconnected");
  ReCnctCount = 0;
}

void reconnectBlynk(){
  bool result = Blynk.connected();
  if(result == false){
    Serial.println("blynk não está conectado!");
     x == false;
     Blynk.begin(auth, ssid, pass);
  }if(result == true){
      Serial.println("blynk está conectado!");
      x == true;
  }else{
      Serial.println("blynk não reconectou!");
      x == false;
      
  }
}

void loop(){
 if(Blynk.connected()){
    Blynk.run();
    Serial.println("blynk rodando");
  }else{
    Blynk.begin(auth, ssid, pass);
  }
  timer.run();
  Serial.println("continua rodando sem blynk?");
  delay(1000);

 /* if (Blynk.connected()) {  // If connected run as normal
    Blynk.run();
  } else if (ReCnctFlag == 0) {  // If NOT connected and not already trying to reconnect, set timer to try to reconnect in 30 seconds
    ReCnctFlag = 1;  // Set reconnection Flag
    Serial.println("Starting reconnection timer in 30 seconds...");
    timer.setTimeout(15000L, []() {  // Lambda Reconnection Timer Function
      ReCnctFlag = 0;  // Reset reconnection Flag
      ReCnctCount++;  // Increment reconnection Counter
      Serial.print("Attempting reconnection #");
      Serial.println(ReCnctCount);
      //Blynk.connect();  // Try to reconnect to the server
      Blynk.begin(auth, ssid, pass);
    });  // END Timer Function
  }
  timer.run();
  Serial.println("continua rodando sem blynk?");/*
  
}
