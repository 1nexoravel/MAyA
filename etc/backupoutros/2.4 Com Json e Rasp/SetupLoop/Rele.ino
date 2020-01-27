int porta_rele1 = 8; // RELE LARANJA
const int buzzer = 3; // buzzer
unsigned long periodo_buz = 2000; // 1 minuto = 60000
unsigned long time_buz    = 0;

void rosca_setup(){
pinMode(porta_rele1, OUTPUT);  // RELE
pinMode(buzzer,OUTPUT);
  digitalWrite(porta_rele1, HIGH); // ROSCA DESLIGADA
  
}

void rosca_desligar(){
  digitalWrite(porta_rele1, HIGH); // DESLIGA A ROSCA
  noTone(buzzer);
}

void rosca_ligar(){
  digitalWrite(porta_rele1, LOW); // LIGA A ROSCA
  if(millis() >  time_buz + periodo_buz){
    time_buz = millis(); 
    tone(buzzer,1500,1000);  
  }
}
  
