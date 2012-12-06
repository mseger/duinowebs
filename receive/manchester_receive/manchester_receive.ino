#include <MANCHESTER.h>

void setup()
{
  // Set digital TX pin
  MANRX_SetRxPin(4);
  // Prepare interrupts
  MANRX_SetupReceive();
  // Begin receiving data
  MANRX_BeginReceive();
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
if (MANRX_ReceiveComplete())
{
  unsigned int data = MANRX_GetMessage();
  MANRX_BeginReceive();
  // Handle data...
  Serial.println(data);
}
}
