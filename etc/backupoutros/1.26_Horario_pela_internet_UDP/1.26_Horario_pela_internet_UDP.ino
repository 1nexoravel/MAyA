#include <NTPClient.h>//Biblioteca do NTP.
#include <WiFiUdp.h>//Biblioteca do UDP.
#include <WiFi.h>//Biblioteca do WiFi.
 
WiFiUDP udp;//Cria um objeto "UDP".
NTPClient ntp(udp, "b.st1.ntp.br", -3 * 3600, 60000);//Cria um objeto "NTP" com as configurações.
const char* ssid     = "inexoravele3e123";
const char* password = "size1080p";


#define led 4//Define o LED ao pino D4.
 
int hora;//Váriavel que armazenara o horario do NTP.
int minuto;

void setup()
{
   Serial.begin(9600);//Inicia a comunicação serial.

 
   pinMode(led, OUTPUT);//Define o pino como saida.
   digitalWrite(led, 1);//Apaga o LED.
 
    WiFi.begin(ssid, password);

    //while (WiFi.status() != WL_CONNECTED) {
    //    delay(500);
   //     Serial.print(".");
   // }
   delay(2000);//Espera a conexão.
 
   ntp.begin();//Inicia o NTP.
   ntp.forceUpdate();//Força o Update.

}
 
void loop()
{
   hora = ntp.getHours(); 
   minuto = ntp.getMinutes(); 
  
  if (ntp.forceUpdate() == false){
   hora = 99;
   minuto = 99;
  }
 
  //hora = 21;
 // minuto = 29;
   Serial.println(hora);
   Serial.println(minuto);

  /* if (hora < 25 && (hora <= 5 || (hora >= 21 && minuto > 30))){
         Serial.println("NÂO");
    }
  hora = 21;
  minuto = 31;
   if (hora < 25 && (hora <= 5 || (hora >= 21 && minuto > 30))){
         Serial.println("AQUI Hora correta 2");
    }

      hora = 29;
  minuto = 31;
   if (hora < 25 && (hora <= 5 || (hora >= 21 && minuto > 30))){
         Serial.println("NÂO 2");
    }

    
      hora = 5;
  minuto = 31;
   if (hora < 25 && (hora <= 5 || (hora >= 21 && minuto > 30))){
         Serial.println("SIM 2");
    }
        hora = 5;
  minuto = 29;
   if (hora < 25 && (hora <= 5 || (hora >= 21 && minuto > 30))){
         Serial.println("SIM 3");
    }
   /*
   if (hora > 5 && (hora <= 21 && minuto <= 30))//Se a hora atual for igual à que definimos, irá acender o led.
   {
      digitalWrite(led, 0);//Acende
      Serial.println("Hora correta");
   }

   minuto = 31;
   if (hora > 5 && (hora <= 21 && minuto <= 30))//Se a hora atual for igual à que definimos, irá acender o led.
   {
      digitalWrite(led, 0);//Acende
      Serial.println("Hora correta 2222");
   }
         if (minuto == 0 || minuto == 10 || minuto == 20 || minuto == 30 || minuto == 40 || minuto == 50)
   {
      digitalWrite(led, 0);//Acende
      Serial.println("enviar bd errado");
   } 
    
    minuto = 0;
  
   if (minuto == 0 || minuto == 10 || minuto == 20 || minuto == 30 || minuto == 40 || minuto == 50)
   {
      digitalWrite(led, 0);//Acende
      Serial.println("enviar bd certo");
   } */

  
   delay(1000);//Espera 1 segundo.
}
