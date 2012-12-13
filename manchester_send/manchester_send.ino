#include <LinkLayer.h>
#include <MANCHESTER.h>



  char destMAC[7] = "nathan";
  char sourceMAC[7] = "allenk";
  unsigned char frameType[3] = {0x08, 0x10, '/0'};
void setup() 
{  
//  MANCHESTER.SetTxPin(4);
////  test.SetTimeOut(1000);
//  Serial.begin(9600);

  int datalength=20;
  int framesize = 14 + datalength + 4;
  LinkLayer mylinklayer(datalength, sourceMAC, destMAC, frameType);

}

int incomingByte = 0;

void loop() {
//  int i;
//  if (Serial.available() > 0) {
//    incomingByte = Serial.read();
//    
//    // say what you got:
//    Serial.print("I sent: ");
//    Serial.println(incomingByte, DEC);
//    
//    // transmit what we typed using manchester encoding
//    MANCHESTER.Transmit(incomingByte);
//  }
}
