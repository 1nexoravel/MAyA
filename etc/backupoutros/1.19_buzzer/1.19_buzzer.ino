const int buzzer = 3; //Constante armazenando o número do pino que vamos utilizar

unsigned long periodo_buz = 2000; // 1 minuto = 60000
unsigned long time_buz    = 0;
 
void setup() {
  /*
    Informamos que ao pino 8 será de saída.
  */
  pinMode(buzzer,OUTPUT);
}
 
void loop() { 
   if(millis() >  time_buz + periodo_buz){
    time_buz = millis(); 
  tone(buzzer,1500,1000);  
}

 /*Em delay esperamos um segundo para avançar para a próxima linha*/
 
  //noTone(buzzer);
  /*Desligamos o Buzzer*/
   
  //delay(1000);
 /*Esperamos um segundo para finalizar o método*/
}
