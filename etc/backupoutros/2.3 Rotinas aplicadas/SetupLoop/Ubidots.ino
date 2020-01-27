#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>

char TOKEN[] = "A1E-yIbCefvS3sWRzmhHsHIAiwwuhHsFRr"; // Assign your Ubidots TOKEN

char DEVICE_LABEL_1[] = "maya-001"; //envia para este device label.

char VARIABLE_LABEL_1[] = "peso_balanca"; // Assign the unique variable label to send the data
char VARIABLE_LABEL_2[] = "tempo"; // Assign the unique variable label to send the data
char VARIABLE_LABEL_3[] = "textos"; // Assign the unique variable label to send the data
char VARIABLE_LABEL_4[] = "gramas"; // Assign the unique variable label to send the data
byte mac[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
/* initialize the instance */
Ubidots client(TOKEN);
float peso_recebido;
void iniciar_ubidots(){
  
  client.setDeviceLabel("maya-001"); //Dispositivo.
  
  Serial.print(F("Starting ethernet..."));
    if (!Ethernet.begin(mac)) {
    Serial.println(F("failed"));
  } else {
    Serial.println(Ethernet.localIP());
  }
  delay(2000);
  Serial.println(F("Ready"));
  Ethernet.maintain();
   client.sendAll();
}

void enviar_ubidots(){
  float valor_1 = peso;
 // float valor_2 = 200;
  
 // Ethernet.maintain();
  client.add(VARIABLE_LABEL_1, valor_1);
 // client.add(VARIABLE_LABEL_4, valor_2);
  client.sendAll();
  Serial.println("Valores enviados!");
 // previousMillis = currentMillis; 
 // contador_1++;
}

void receber_ubidots(){
  peso_recebido = client.getValue(DEVICE_LABEL_1, VARIABLE_LABEL_4);
//  tempo_recebido = client.getValue(DEVICE_LABEL_1, VARIABLE_LABEL_2);
  Serial.print("Recebido:");
  Serial.println(peso_recebido);
  //previousMillis = currentMillis; 
 // contador_1 = 0;
 
}
