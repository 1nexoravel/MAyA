// Vai ser usado para movimentar a porta para a saida de ração para a balança

#include <Servo.h>
#include "HX711.h"                    // Biblioteca HX711 
 
#define DOUT  A0                  // LARANJA    

#define CLK  A1                 // AZUL       

Servo name_servo;               //OBJETO DO TIPO SERVO
int servo_position = 0;
HX711 balanca(DOUT, CLK);   // instancia Balança HX711
 
float calibration_factor = 400000;   // fator de calibração aferido na Calibraçao 

int porta_rele1 = 7; // RELE LARANJA


///////////////////////////////
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
char const * TOKEN = "BBFF-CXzATMBGNp8TeHjBpYt2lrPp5Hy7zD"; // Assign your Ubidots TOKEN
char const * VARIABLE_LABEL_1 = "Peso"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_2 = "humidity"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_3 = "pressure"; // Assign the unique variable label to send the data
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
/* initialize the instance */
Ubidots client(TOKEN);
/////////////////////////////////



void setup() {
 Serial.begin(9600);
 
  pinMode(porta_rele1, OUTPUT);  // RELE
  digitalWrite(porta_rele1, HIGH);
  name_servo.attach (9); // SERVO // LARANJA
 Serial.println("Balança com HX711 - celula de carga 50 Kg"); 
  Serial.println("Pressione t para Tara");   // imprime no monitor serial
  balanca.set_scale(calibration_factor);   // ajusta fator de calibração
  balanca.tare();   // zera a Balança
  Serial.print(F("Starting ethernet..."));
  if (!Ethernet.begin(mac)) {
    Serial.println(F("failed"));
  } else {
    Serial.println(Ethernet.localIP());
  }
  delay(2000);
  Serial.println(F("Ready"));
}
 
void loop() {
  
Serial.print("Peso: ");   // imprime no monitor serial
  Serial.print(balanca.get_units(5), 3);   // imprime peso na balança com 3 casas decimais 
  Serial.println(" kg");   // imprime no monitor serial 
  //delay(500) ;   // atraso de 0,5 segundos
float peso = balanca.get_units(5);
 //Serial.print("LINHA TESTE: ");
////////////////////////
Ethernet.maintain();
//float value_1 = analogRead(A0);
  //float value_2 = analogRead(A1);
 // float value_3 = analogRead(A2);
  /* Sending values to Ubidots */
  client.add(VARIABLE_LABEL_1, peso);
  //client.add(VARIABLE_LABEL_2, value_2);
  //client.add(VARIABLE_LABEL_3, value_3);
  client.sendAll();
  delay(5000);
////////////////////////////

 if (Serial.available()) {
     char temp = Serial.read();
  if (temp == 'a' || temp == 'A') {
   for (servo_position = 0; servo_position <=180; servo_position +=1){
    name_servo.write(servo_position);
    delay(10);
  }
   Serial.println(" Rotação feita");   
 }

  if (temp == 's' || temp == 'S') {
   for (servo_position=180; servo_position >= 0; servo_position -=1){
    name_servo.write(servo_position);
    delay(10);
  }
   Serial.println("Rotação feita");   
   }
 
 if (temp == 'r' || temp == 'R') {
   digitalWrite(porta_rele1, LOW); // LIGA A ROSCA
   Serial.println("MOTOR DC LIGADO(ROSCA INFINITA)"); 
  }   // IF DOS PESOS NA BALANÇA
  if (peso >= 0.2){ //EQUIVALE A 200g, COLOCAR ROTACAO DO SERVO AQUI DENTRO, FALTA SABER QUAL LADO DEPOIS DE APREGAR O SERVO
    digitalWrite(porta_rele1, HIGH); // DESLIGA A ROSCA
   Serial.println("MOTOR DC DESLIGADO(ROSCA INFINITA)");  
  }
 }
 }
 
//  if (Serial.available())   // reconhece letra para ajuste do fator de calibração
//  {
//   char temp = Serial.read();
//   if (temp == '+' || temp == 'a')   // a = aumenta 10
//   calibration_factor += 10;
//   else if (temp == '-' || temp == 'z')   // z = diminui 10
//   calibration_factor -= 10;
//   else if (temp == 's')   // s = aumenta 100
//   calibration_factor += 100;
//   else if (temp == 'x')   // x = diminui 100
//   calibration_factor -= 100;
//   else if (temp == 'd')   // d = aumenta 1000
//   calibration_factor += 1000;
//   else if (temp == 'c')   // c = diminui 1000
//   calibration_factor -= 1000;
//   else if (temp == 'f')   // f = aumenta 10000
//   calibration_factor += 10000;
//   else if (temp == 'v')   // v = dimuni 10000
//   calibration_factor -= 10000;
//   else if (temp == 't') zeraBalanca ();   // t = zera a Balança
//  }
//
//   if ((x > 1) && (servo_position < 50)) {
//   for (servo_position = 0; servo_position <=180; servo_position +=1){
//    name_servo.write(servo_position);
//    delay(10);
//  }
//   Serial.println(" Rotação feita");   
// }
//
//  if ((x <= 1) && (servo_position > 130)) {
//   for (servo_position=180; servo_position >= 0; servo_position -=1){
//    name_servo.write(servo_position);
//    delay(10);
//  }
//   Serial.println("Rotação feita");   
//   }
