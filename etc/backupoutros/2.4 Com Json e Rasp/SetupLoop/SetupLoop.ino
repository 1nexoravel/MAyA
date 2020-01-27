String teste;
float voltage;
float degreesC;
int led1 = 11;
int led2 = 12;
int led3 = 13;


const uint32_t debounceTime = 1000;  // 5 mSec, enough for most switches
const uint8_t switchPin     = 4;  // with n.o. momentary pb switch to ground
const uint8_t ledPin        = 13; // built-in led

const bool switchOn  = false;     // using INPUT_PULLUP
const bool switchOff = true;

bool lastState   = switchOff;
bool newState    = switchOff;
bool toggleState = false;

void setup() {
  leds();
   pinMode ( switchPin, INPUT_PULLUP );
  servo_pino();
  servo_balanca();
  Serial.begin(9600); 
  balanca_setup();
  rosca_setup();
  datahora_setup();
  
}

void loop() {
  balanca_loop();
  request_rasp();
  response_rasp();
  temperatura();
  datahora_loop();
  
//  if(teste.equals("despejar")){ // ESSA É SEM O BOTAO, VERIF var quantidade se é global depois~
//    despejar_comida(); 
//  }
  newState = digitalRead( switchPin );

  if( lastState != newState ) // state changed
  {
    delay( debounceTime );
    lastState = newState;
    
    // push on, push off
    if( newState == switchOn && toggleState == false )
    {
      toggleState = true;
      despejar_comida(); 
      Serial.println( F ( "ATIVADO" ) );
    }
    else if( newState == switchOn && toggleState == true )
    {
      toggleState = false;
      Serial.println( F ( "DESATIVADO" ) );
    }

}
}
