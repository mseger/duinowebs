/*
  State change detection (edge detection)
 	
 Often, you don't need to know the state of a digital input all the time,
 but you just need to know when the input changes from one state to another.
 For example, you want to know when a button goes from OFF to ON.  This is called
 state change detection, or edge detection.
 
 This example shows how to detect when a button or button changes from off to on
 and on to off.
 	
 The circuit:
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 * LED attached from pin 13 to ground (or use the built-in LED on
   most Arduino boards)
 
 created  27 Sep 2005
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.
 	
 http://arduino.cc/en/Tutorial/ButtonStateChange
 
 */

// this constant won't change:
const int  buttonPin = 2;    // the pin that the pushbutton is attached to
const int ledPin = 13;       // the pin that the LED is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int bitCount = 0;
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 10;           // interval at which to blink (milliseconds)


void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  //Serial.begin(9600);
}


void loop() {
  
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   


    buttonState = digitalRead(buttonPin);
  
  
  // read the pushbutton input pin:

      buttonPushCounter++;
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
    digitalWrite(ledPin, HIGH);
      
    } 
    else {
      // if the current state is LOW then the button
      // wend from on to off:
   digitalWrite(ledPin, LOW);
    }
  if (buttonState!=lastButtonState)
  {
    
    
//      Serial.print(buttonPushCounter);
//    if (buttonState==HIGH){
//      Serial.println("on");
//    }
//    else {      
//      Serial.println("off");
//    }
    bitCount++;
    //Serial.print(bitCount);
    buttonPushCounter=0;
  }
  if ((buttonPushCounter>150)&&(buttonState==lastButtonState))
  {
    
    
//      Serial.print(bitCount);
//    if (buttonState==HIGH){
//      Serial.println("double on");
//    }
//    else {      
//      Serial.println("double off");
//    }
    buttonPushCounter=0;
    bitCount=0;
  }
  // save the current state as the last state, 
  //for next time through the loo
  
  lastButtonState = buttonState;

  }
}









