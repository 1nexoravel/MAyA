int porta_rele1 = 7; // RELE LARANJA

void rosca_setup(){
pinMode(porta_rele1, OUTPUT);  // RELE
  digitalWrite(porta_rele1, HIGH); // ROSCA DESLIGADA
}

void rosca_desligar(){
  digitalWrite(porta_rele1, HIGH); // DESLIGA A ROSCA
}

void rosca_ligar(){
  digitalWrite(porta_rele1, LOW); // DESLIGA A ROSCA
}
  
