#include "HX711.h"                    // Biblioteca HX711 
#define DOUT  A0                 // LARANJA    
#define CLK  A1                // AZUL       

HX711 balanca(DOUT, CLK);   // instancia Balança HX711
float calibration_factor = 420000; 

float peso;

void balanca_setup(){
  balanca.set_scale(calibration_factor);   // ajusta fator de calibração
 // Serial.println("Fator de calibração efetuado.");
  balanca.tare();   // zera a Balança
 // Serial.println("Tara zerada.");
 // Serial.println("PRONTO!");
  }

void balanca_loop(){
  peso = balanca.get_units(5);
  if ((peso > 3 || peso < -1)){
    digitalWrite(led1, HIGH); 
    }
 // Serial.print("Peso: ");   // imprime no monitor serial
  //Serial.println(balanca.get_units(5), 3);   // imprime peso na balança com 3 casas decimais 
 // Serial.println(" kg");   // imprime no monitor serial 
  //delay(500) ;   // atraso de 0,5 segundos

}
