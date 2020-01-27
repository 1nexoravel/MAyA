//long previousMillis = 0;        // Variável de controle do tempo
//
//long intervalo = ;


void setup() {
  servo_pino();
  servo_balanca();
  Serial.begin(9600);
  
  //  iniciar_ubidots();
  
  balanca_setup();
  rosca_setup();
 // cartao_setup();
  
}

void loop() {
   request_nuvem();
   response_nuvem();
balanca_loop();
  if (Serial.available()) {
    char c = Serial.read();
        if (c == 'a' || c == 'A')
        despejar_comida();
   }  
}
