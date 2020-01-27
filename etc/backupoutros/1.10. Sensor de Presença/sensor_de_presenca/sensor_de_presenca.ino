//Declaração de portas digitais aos quais os led e a entrada digital do sensor vão estar conectados.
int ledverde = 3; 
int ledvermelho = 2;
int pinoSensor = 7;
//Inicia o sensor em estado 0, ou seja desligado.
int iniciaSensor = 0;
//Variável para calibração do sensor
int calibracao = 45;
 
void setup() {
Serial.begin(9600);
 
//Declaração de que os LED's serão usados como saída.
pinMode(ledvermelho,OUTPUT);
pinMode(ledverde,OUTPUT);
 
pinMode(pinoSensor,INPUT); //E o sensor uma entrada.
Serial.print("Calibrando o sensor...");
for(int i = 0; i < calibracao; i++){
Serial.print(".");
delay(1000);
}
Serial.println("Sensor Ativado");
delay(500);
}
 
void loop() {
//A cada ciclo, ele faz uma nova leitura do estado do sensor de presença 
iniciaSensor = digitalRead(pinoSensor);
 
Serial.print("Valor do Sensor PIR: "); 
Serial.println(iniciaSensor);
 
////Verificando se ocorreu detecção de movimentos
if (iniciaSensor == 1) {
alarme_on();
} else {
alarme_off();
} 
}
 
void alarme_on() {  //Função que ativa o alarme - Detectou presença, o LED Vermelho fica acende
//E o LED VERDE apaga.
digitalWrite(ledvermelho, HIGH);
digitalWrite(ledverde, LOW); 
 
}
void alarme_off() { //Função que desativa o alarme - N foi detectado presença, o LED Vermelho apaga
//E o LED VERDE acende. 
digitalWrite(ledvermelho, LOW);
digitalWrite(ledverde, HIGH);
}
