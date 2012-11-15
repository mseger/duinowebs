/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
#include <string.h>

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}


void preamble(char inputstr[], long delaytime) {
  int i;
  for(i=0; i<strlen(inputstr);i++) {     
     if (inputstr[i] == '0'){
       digitalWrite(13, LOW);    // set the LED off
       digitalWrite(9, LOW);           
       Serial.println("low");
     }
     
     else{
       digitalWrite(13, HIGH);    // set the LED on
       digitalWrite(9, HIGH);           
       Serial.println("high");
     }
     delay(delaytime);
  }
}



void loop() {
  preamble("0101011", 1000);
}
