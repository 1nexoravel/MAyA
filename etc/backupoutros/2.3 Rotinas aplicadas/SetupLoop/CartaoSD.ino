//#include <SD.h>
//File myFile;
//
//void cartao_setup(){
//  if (!SD.begin(4)) {
//   // Serial.println("initialization failed!");
//    while (1);
//   // Serial.println("initialization done.");
//  }
//}
//
//void cartao_escrever(){ //escreve e já fecha
//  myFile = SD.open("teste.txt", FILE_WRITE);
//
//  // if the file opened okay, write to it:
//  if (myFile) {
//   // Serial.print("Writing to test.txt...");
//    myFile.println("testing 1, 2, 3."); // vai escrever isso
//    // close the file:
//    myFile.close();   //fecha o arquivo
//  //  Serial.println("done.");
//  } else {
//    // if the file didn't open, print an error:
//  //  Serial.println("error opening test.txt");
//  }
//}
//
//void cartao_ler(){
//  // re-open the file for reading:
//  myFile = SD.open("test.txt");
//  if (myFile) {
//  //  Serial.println("test.txt:");
//
//    // read from the file until there's nothing else in it:
//    while (myFile.available()) {
//   //   Serial.write(myFile.read());
//    }
//    // close the file:
//    myFile.close();
//  } else {
//    // if the file didn't open, print an error:
//   // Serial.println("error opening test.txt");
//  }
//}
