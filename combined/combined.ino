#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MANCHESTER.h>
#include <avr/pgmspace.h>



//CRC code
static PROGMEM prog_uint32_t crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

unsigned long crc_update(unsigned long crc, byte data)
{
    byte tbl_idx;
    tbl_idx = crc ^ (data >> (0 * 4));
    crc = pgm_read_dword_near(crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);
    tbl_idx = crc ^ (data >> (1 * 4));
    crc = pgm_read_dword_near(crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);
    return crc;
}

unsigned long crc_string(char *s)
{
  unsigned long crc = ~0L;
  while (*s)
    crc = crc_update(crc, *s++);
  crc = ~crc;
  return crc;
}


//Combining code
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
    Serial.print(destMAC[i]);
  }  
  Serial.println();
  int j;
  for(j=0; j<6; j++){
    all_appended[j+6] = sourceMAC[j];
    Serial.print(sourceMAC[j]);
  }
  Serial.println();
  
  int z;
  for(z=0; z<2; z++){
     all_appended[z+12] = frameType[z];
     Serial.print(frameType[z]);
  }
  Serial.println();  
  int k;
  for(k=0; k<50; k++){
    all_appended[k+14] = data[k];
    Serial.print(data[k]);
  }
  Serial.println();
  return all_appended;
}




void setup()
{
  MANCHESTER.SetTxPin(4);
  Serial.begin(9600);
    int i;
    unsigned char *appended = append_all();
//  long crc = crc_string((char *) appended);
  long crc = crc_string((char *) data);
  for (i=0; i<65; i++){
      Serial.print( data[i], HEX);
  }
//  for(i=0; i<64; i++){
//    Serial.print((char) appended[i], HEX);
//  }
  Serial.println("");
  Serial.println(crc,HEX);
}

void loop()
{  
//  data = "0x016328576189326486108932750917 328461875610893724089136507612 384710923787165081273409812387 561822374196378916051092365081 033561872346871263401275061203 468172635409162358716234096128 735710326401635087163486123071 023460812365871234701236510823 236510872364081726350127341256"

  
//  MANCHESTER.Transmit(incomingByte);
}
