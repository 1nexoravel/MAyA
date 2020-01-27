#include <Servo.h>
Servo nome_servo;               
int posicao_servo = 90;

void servo_pino(){
  nome_servo.attach (9); // SERVO // LARANJA // PORTA 9
  }

void servo_despejar(){
  for (posicao_servo = 50; posicao_servo <=140; posicao_servo +=1){
    nome_servo.write(posicao_servo);
    delay(10);
 }
 Serial.println("Rotação feita");  
}

void servo_balanca(){
  for (posicao_servo=140; posicao_servo >= 50; posicao_servo -=1){
    nome_servo.write(posicao_servo);
    delay(10);
 }
 Serial.println("Rotação feita");  
}
