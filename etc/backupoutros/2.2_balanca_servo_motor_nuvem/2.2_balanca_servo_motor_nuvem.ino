// Vai ser usado para movimentar a porta para a saida de ração para a balança
//#include <Servo.h>
//#include "HX711.h"                    // Biblioteca HX711 
 
//#define DOUT  A0                  // LARANJA    
//#define CLK  A1                 // AZUL       

//Servo name_servo;               //OBJETO DO TIPO SERVO
//int servo_position = 0;

//HX711 balanca(DOUT, CLK);   // instancia Balança HX711
// 
//float calibration_factor = 400000;   // fator de calibração aferido na Calibraçao 

int porta_rele1 = 7; // RELE LARANJA


///////////////////////////////
//#include <Ethernet.h>
//#include <SPI.h>
//#include <UbidotsEthernet.h>
//char TOKEN[] = "A1E-yIbCefvS3sWRzmhHsHIAiwwuhHsFRr"; // Assign your Ubidots TOKEN
//char DEVICE_LABEL[] = "maya-001";
//char VARIABLE_LABEL[] = "rui"; // Assign the unique variable label to send the data
//char VARIABLE_LABEL_1[] = "peso"; // Assign the unique variable label to send the data
//char VARIABLE_LABEL_2[] = "tempo"; // Assign the unique variable label to send the data
//char VARIABLE_LABEL_3[] = "textos"; // Assign the unique variable label to send the data
//byte mac[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
///* initialize the instance */
//Ubidots client(TOKEN);
/////////////////////////////////
float tempo;


void setup() {
  tempo = 1;
//  client.setDeviceLabel("maya-001");  //muda o label
// Serial.begin(9600);
 
  pinMode(porta_rele1, OUTPUT);  // RELE
  digitalWrite(porta_rele1, HIGH); // ROSCA DESLIGADA
  //name_servo.attach (9); // SERVO // LARANJA
  
// Serial.println("Balança com HX711 - celula de carga 50 Kg"); 
//  Serial.println("Pressione t para Tara");   // imprime no monitor serial
//  balanca.set_scale(calibration_factor);   // ajusta fator de calibração
//  balanca.tare();   // zera a Balança
//  Serial.print(F("Starting ethernet..."));
//  if (!Ethernet.begin(mac)) {
//    Serial.println(F("failed"));
//  } else {
//    Serial.println(Ethernet.localIP());
//  }
//  delay(2000);
//  Serial.println(F("Ready"));
}
 
void loop() {
  
Serial.print("Peso: ");   // imprime no monitor serial
  Serial.print(balanca.get_units(5), 3);   // imprime peso na balança com 3 casas decimais 
  Serial.println(" kg");   // imprime no monitor serial 
  //delay(500) ;   // atraso de 0,5 segundos
float peso = balanca.get_units(5);

 //Serial.print("LINHA TESTE: ");
////////////////////////
//Ethernet.maintain();
////  if (tempo == 10){
////float value_1 = analogRead(A0);
//  //float value_2 = analogRead(A1);
//  // float value_3 = analogRead(A2);
//  /* Sending values to Ubidots */
//  unsigned long timestamp = 1537453824;
//  client.add(VARIABLE_LABEL_1, peso);
//  client.add(VARIABLE_LABEL_2, tempo, timestamp);
///////////////////////////////////////////////////////////////
// // char context[80];
// // sprintf(context, " \"lat\":1.2343, \"lng\":132.1233, \"texto\":12345 ");
// // client.add(VARIABLE_LABEL_3, 2, context);
////////////////////////////////////////////////////////////
//
//
//
//  client.sendAll();
// float data = client.getValue(DEVICE_LABEL, VARIABLE_LABEL);
//  //}
//  //tempo++;

// Serial.print("o valor lido é:  ");
//    Serial.println(data);
//    delay(5000);
////////////////////////////

 if (Serial.available()) {
     char c = Serial.read();
  if (c == 'a' || c == 'A') {
   for (servo_position = 0; servo_position <=180; servo_position +=1){
    name_servo.write(servo_position);
    delay(10);
  }
   Serial.println(" Rotação feita");   
 }

  if (c == 's' || c == 'S') {
   for (servo_position=180; servo_position >= 0; servo_position -=1){
    name_servo.write(servo_position);
    delay(10);
  }
   Serial.println("Rotação feita");   
   }
 
 if (c == 'r' || c == 'R') {
   digitalWrite(porta_rele1, LOW); // LIGA A ROSCA
   Serial.println("MOTOR DC LIGADO(ROSCA INFINITA)"); 
  }   // IF DOS PESOS NA BALANÇA
  if (peso >= 0.2){ //EQUIVALE A 200g, COLOCAR ROTACAO DO SERVO AQUI DENTRO, FALTA SABER QUAL LADO DEPOIS DE APREGAR O SERVO
    digitalWrite(porta_rele1, HIGH); // DESLIGA A ROSCA
   Serial.println("MOTOR DC DESLIGADO(ROSCA INFINITA)");  
  
   for (servo_position=180; servo_position >= 0; servo_position -=1){
    name_servo.write(servo_position);
    delay(10);
   }
   Serial.println("Rotação feita");  
   delay(3000);
  for (servo_position = 0; servo_position <=180; servo_position +=1){
    name_servo.write(servo_position);
    delay(10);
  }
   Serial.println(" Rotação feita");  
   
   }
   
  }
  
 }
 
