int LED = 13;
int SENSOR = A0; // Pino que irá receber o sinal do fototransistor
int valor;
void setup() {
 Serial.begin(9600);
 pinMode(LED, OUTPUT);
}
void loop() {
 // Obter o valor do sensor (entre 0 e 1023-entrada analógica)
 valor = analogRead(SENSOR);
 Serial.print("Valor: ");
 Serial.println(valor);
 if (valor > 300)
 digitalWrite(LED, HIGH);
 else
 digitalWrite(LED, LOW);
 delay (500);
}
