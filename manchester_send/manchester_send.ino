#include <MANCHESTER.h>

void setup() 
{  
  MANCHESTER.SetTxPin(4);
//  test.SetTimeOut(1000);
  Serial.begin(9600);
}

int incomingByte = 0;

void loop() {
  int i;
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    
    // say what you got:
    Serial.print("I sent: ");
    Serial.println(incomingByte, DEC);
    
    // transmit what we typed using manchester encoding
    MANCHESTER.Transmit(incomingByte);
  }
}
