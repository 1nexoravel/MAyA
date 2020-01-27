int pinoSensor = A5;
 int sensor;
void setup() {
Serial.begin(9600);
pinMode(pinoSensor,INPUT); //E o sensor uma entrada.
}
void loop() {
  sensor = analogRead(pinoSensor);
  Serial.println(sensor);
  delay(100);
}
 
