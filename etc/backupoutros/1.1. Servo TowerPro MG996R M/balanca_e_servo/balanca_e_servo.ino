// Vai ser usado para movimentar a porta para a saida de ração para a balança

#include <Servo.h>
#include "HX711.h"                    // Biblioteca HX711 
 
#define DOUT  A0                      // HX711 DATA OUT = pino A0 do Arduino 
#define CLK  A1                       // HX711 SCK IN = pino A1 do Arduino 
 
Servo name_servo;
 
int servo_position = 0;

HX711 balanca(DOUT, CLK);   // instancia Balança HX711
 
float calibration_factor = 1000;   // fator de calibração aferido na Calibraçao 
 int x = 0;
void setup() {
 Serial.begin(9600);
  name_servo.attach (9);
 Serial.println("Balança com HX711 - celula de carga 50 Kg"); 
  Serial.println("Pressione t para Tara");   // imprime no monitor serial
  balanca.set_scale(calibration_factor);   // ajusta fator de calibração
  balanca.tare();   // zera a Balança
}
 
void loop() {
  // if (Serial.available()) {
Serial.print("Peso: ");   // imprime no monitor serial
  Serial.print(balanca.get_units(50));   // imprime peso na balança com 3 casas decimais 
  Serial.println(" kg");   // imprime no monitor serial 
  //delay(500) ;   // atraso de 0,5 segundos
  x = balanca.get_units(50);
  Serial.println(x);
 
     
   if ((x > 1) && (servo_position < 50)) {
   for (servo_position = 0; servo_position <=180; servo_position +=1){
    name_servo.write(servo_position);
    delay(10);
  }
   Serial.println(" Rotação feita");   
 }

  if ((x <= 1) && (servo_position > 130)) {
   for (servo_position=180; servo_position >= 0; servo_position -=1){
    name_servo.write(servo_position);
    delay(10);
  }
   Serial.println("Rotação feita");   
   }
 // }
}
