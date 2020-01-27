#include <user_config.h>
#include <SmingCore/SmingCore.h>
 
//endereço i2c
#define MPU6050_ADDR 0x68
#define WHO_I_AM     0x75
#define SCALE        1000.0
 
//variaveis para armazenamento das leituras
int accelX        = 0;
int accelY        = 0;
int accelZ        = 0;
int girosX        = 0;
int girosY        = 0;
int girosZ        = 0;
int temp          = 0;
float temperature = 0.0;
 
float sensors[7] = {0};
 
//execucao da coleta de dados via timer
Timer getData;
 
/* O uso de delay nao eh recomendado porque pode gerar reset por WDT. Utilizando
 * a funcao millis() a CPU fica livre pra executar todos os processos de 
 * background sem interrupcoes.
 */
void sleep(int t){
    long start = 0;
    long now  = 0;
    int  delta = 0;
    
    start = millis();
    while (delta < t){
        now   = millis();
        delta = now - start;
        WDT.alive();
    }
}
 
//leitura do registrador do MPU6050
void mpu6050_read(){
    Wire.beginTransmission(MPU6050_ADDR);
    //para pegar o MSB (bit 8 a 15)
    Wire.write(0x3B);
    Wire.endTransmission(false);
    //requisita os dados e finaliza a transmissao
    Wire.requestFrom(MPU6050_ADDR,14,true);
    /* O MPU6050 tem um sensor de temperatura. Seu valor esta sendo lido acima,
     mas ainda eh necessario aplicar uma pequena formula para converter o valor
     para temperatura legivel em graus Celsius. Isso esta descrito na pagina 31
     do documento contendo o mapa de registradores. O link esta no artigo.
     Os enderecos do acelerometro estao na pagina 30.
     Os enderecos da temperatura estao na pagina 31.
     * Os enderecos do gyroscopio estao na pagina 32.
     * MSB representa os 8 bits mais signifcantes (Most Significant Bits)
     * LSB representa os 8 bits menos significantes (Least Significant Bits)
     * Abaixo, le um byte e desloca ele para a esquerda, entao concatena o
     * proximo byte.
     */
    accelX  = Wire.read()<<8;
    accelX |= Wire.read();   
    accelY  = Wire.read()<<8;
    accelY |= Wire.read(); 
    accelZ  = Wire.read()<<8;
    accelZ |= Wire.read();  
    temp    = Wire.read()<<8;
    temp   |= Wire.read();  
    girosX  = Wire.read()<<8;
    girosX |= Wire.read();  
    girosY  = Wire.read()<<8;
    girosY |= Wire.read();  
    girosZ  = Wire.read()<<8;
    girosZ |= Wire.read();  
    
    temp        = temp/340.0 + 36.53;
    //temperature = temp/10.0;
    
    sensors[0]  = accelX/SCALE;
    sensors[1]  = accelY/SCALE;
    sensors[2]  = accelZ/SCALE;
    sensors[3]  = girosX/SCALE;
    sensors[4]  = girosY/SCALE;
    sensors[5]  = girosZ/SCALE;
    sensors[6]  = temp/10.0;
}
 
/* PWR_MGMT_1
 * Esse eh o registrador 107 (ou 0x6B em hexa), descrito na pagina 48 do mapa
 * de registradores. A configuracao inicial eh bastante simples, e os bits estao
 * descritos no artigo.
 * 
 */
void init_mpu6050(){
    int regs[] = {107,0x1B,0x1C};
    for (int i=0;i<3;i++){
        Wire.beginTransmission(MPU6050_ADDR);
        Wire.write(regs[i]);
        Wire.write(0);
        Wire.endTransmission(true);
        sleep(100);
    }
//    Wire.beginTransmission(MPU6050_ADDR);
//    Wire.write(108);
//    Wire.write(3);
//    Wire.endTransmission(true);
}
 
void whoIam(){
    Wire.beginTransmission(MPU6050_ADDR);
        Wire.write(WHO_I_AM);
        Wire.requestFrom(MPU6050_ADDR,1,true);
        int identity = Wire.read();
        Serial.print("Identificacao: ");
        Serial.println(identity);
}
 
int scan(){
    int error;
    for (int i=1;i<127;i++){
        Serial.print("Endereco: ");
        Serial.println(i);
        Wire.beginTransmission(i);
        error = Wire.endTransmission(true);
        Serial.print("Codigo de retorno: ");
        Serial.println(error);
        WDT.alive();
        if (error == 0){
            return i;
        }
    }
    return 0;
}
 
int mpu6050_status(){
    /* Verifica se o dispositivo responde em MPU6050_ADDR*/
    Wire.beginTransmission(MPU6050_ADDR); 
    int error = -1; 
    error = Wire.endTransmission(true);
    if (error == 0){
        Serial.println("Dispositivo Ok!");
        return 0;
    }
    return -1;
}
 
void enableInterrupt(int threshold, int axis){
    //ativa interrupcao
    Wire.beginTransmission(MPU6050_ADDR);     
    Wire.write(56);
    Wire.write(0b01000000);         
    Wire.endTransmission(true);
  
    //gatilho
    Wire.beginTransmission(MPU6050_ADDR);     
    Wire.write(0x1F);
    Wire.write(threshold); //1 eh o minimo         
    Wire.endTransmission(true);
}
 
void showValues(){
    Serial.print("Acelerometro (X): ");
    Serial.println(sensors[0]);
    Serial.print("Acelerometro (Y): ");
    Serial.println(sensors[1]);
    Serial.print("Acelerometro (Z): ");
    Serial.println(sensors[2]);
    Serial.print("Giroscopio (X): ");
    Serial.println(sensors[3]);
    Serial.print("Giroscopio (Y): ");
    Serial.println(sensors[4]);
    Serial.print("Giroscopio (Z): ");
    Serial.println(sensors[5]);
    Serial.print("Temperatura: ");
    Serial.println(sensors[6]);
}
 
/* Essa funcao serve apenas para verificar se o MPU6050 saiu do modo sleep.*/
int isSleeping(){
  int val;
  Wire.beginTransmission(MPU6050_ADDR);     
  Wire.write(0x6B);                       //registro do gerenciamento de energia                     
  Wire.endTransmission(false);            //nao finalizar anter de ler         
  Wire.requestFrom(MPU6050_ADDR, 1,true); //le um byte  
  val = Wire.read();                      //le a resposta
  return val;                          
}
 
void mpu6050_getData(){
    whoIam();
    accelX = 0;
    int t = Wire.available();
    //Serial.print("Available? ");
    //Serial.println(t);
    while (accelX == 0){
        mpu6050_read();
        showValues();
    }
    int stat = -1;
    stat = isSleeping();
    if (stat != 64){
        //Serial.print("Dormindo como um bebe... (");
        //Serial.print(stat);
        //Serial.println(")");
        int _null = 0;
    }
    //whoIam();
}
 
void init()
{
    Serial.begin(115200);
    //inicia o barramento
    Wire.pins(12,14);
    Wire.begin();
    sleep(100);
    int isThereOne = scan();
    
    //valida o dispositivo
    int found;
    found = mpu6050_status();
    if (found != 0){
        Serial.println("Dispositivo nao detectado. Abortando");
        Serial.println(found);
        return;
    }
    
    Serial.println("inicializando MPU6050");
    //inicia o MPU6050
    init_mpu6050();
    sleep(100);
    
    //faz leituras periodicas
    Serial.println("Inicializando timer para 5 segundos");
    getData.initializeMs(5000, mpu6050_getData).start(); 
}
