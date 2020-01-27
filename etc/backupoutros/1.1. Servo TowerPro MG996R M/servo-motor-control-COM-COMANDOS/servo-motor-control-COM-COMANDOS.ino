// Vai ser usado para movimentar a porta para a saida de ração para a balança

#include <Servo.h>
 
Servo name_servo;
 
int servo_position = 90;
 
void setup() {
 Serial.begin(9600);
  name_servo.attach (9);
 
}
 
void loop() {

  if (Serial.available()) {
   char temp = Serial.read();   
   if (temp == 'a' || temp == 'A') {// abrir.
   for (servo_position = 50; servo_position <=140; servo_position +=1){
    name_servo.write(servo_position);
    delay(10);
  }
   Serial.println(" Rotação feita");   
 }

  if (temp == 's' || temp == 'S') { //fechar
   for (servo_position=140; servo_position >= 50; servo_position -=1){
    name_servo.write(servo_position);
    delay(10);
  }
   Serial.println("Rotação feita");   
   }
  }
}
