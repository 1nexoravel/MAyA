int period = 1000;
unsigned long time_now = 0;
int contador_1 = 0;
//timestamp

void request_nuvem(){ //enviar p nuvem
      if(millis() > time_now + period){ //envio
        time_now = millis(); 
    enviar_ubidots();
    contador_1++;
  }
}

void response_nuvem(){ //receber da nuvem
    if(contador_1 == 6) { // recebimento de dados a cada 1 minuto.
    receber_ubidots();
    contador_1 = 0;
  }
}

void despejar_comida(){
  //if(tempo_recebido == timestamp){
    while(peso <= 0.1){
    balanca_loop();
    rosca_ligar();
    } 
    rosca_desligar();
    delay(3000);
    servo_despejar();
    delay(3000);
    servo_balanca();
    //balanca_setup;
  //}
}
