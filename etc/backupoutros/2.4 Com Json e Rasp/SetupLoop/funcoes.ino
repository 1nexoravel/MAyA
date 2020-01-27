unsigned long periodo_req = 6000; // 1 minuto = 60000
unsigned long time_req    = 0;
//timestamp
unsigned long periodo_res = 30000; // 1 minuto = 60000
unsigned long time_res    = 0;




void request_rasp(){ //enviar p raspberry
   if(millis() >  time_req + periodo_req){
    time_req = millis(); 
      JsonEnviar();
      
  }
}

void response_rasp(){ //receber do raspberry
   if(millis() >  time_res + periodo_res){
    time_res = millis(); 
    JsonReceber();
  }
}

void despejar_comida(){
  //if(tempo_recebido == timestamp){
    while(peso <= quantidade){
    balanca_loop();
    rosca_ligar();
    }
    rosca_desligar();
    delay(3000);
    servo_despejar();
    delay(5000);
    servo_balanca();
    teste = "gg";
  //}
}
