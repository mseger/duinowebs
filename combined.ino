#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MANCHESTER.h>

void setup()
{
  MANCHESTER.SetTxPin(4);
  Serial.begin(9600);
}

int incomingByte = 0;

unsigned char *data_generation()
{  
  unsigned char data[51];
  int i;
  for(i=0;i<50;i++)
  {
    data[i] = (unsigned char) random(10);
  }
  return data;
}

unsigned char destMAC[7] = {0x92, 0x81, 0x22, 0xab, 0x3f, 0xfa};
unsigned char sourceMAC[7] = {0x28, 0x11, 0xda, 0x34, 0x4b, 0x2d};
unsigned char frameType[3] = {0x08, 0x00};
unsigned char *data = data_generation();

unsigned char *append_all(){
  unsigned char all_appended[65]; 
  
  int i;
  for(i=0; i<6; i++){
    all_appended[i] = destMAC[i];
  }  
  
  int j;
  for(j=0; j<6; j++){
    all_appended[j+6] = sourceMAC[j];
  }
  
  int z;
  for(z=0; z<2; z++){
     all_appended[z+12] = sourceMAC[z];
  }
  
  int k;
  for(k=0; k<50; k++){
    all_appended[k+14] = data[k];
  }
  
  return all_appended;
}

unsigned char *appended = append_all();

void loop()
{  
//  data = "0x016328576189326486108932750917 328461875610893724089136507612 384710923787165081273409812387 561822374196378916051092365081 033561872346871263401275061203 468172635409162358716234096128 735710326401635087163486123071 023460812365871234701236510823 236510872364081726350127341256"
  int i;
  for(i=0; i<64; i++){
    Serial.print(appended[i]);
  }
  Serial.println("");
  
  MANCHESTER.Transmit(incomingByte);
}
