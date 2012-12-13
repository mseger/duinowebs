#include "MANCHESTER.h"

#include "Arduino.h"
#include "LinkLayer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

LinkLayer::LinkLayer(int inputDatalength, char *inputSourceMAC, char *inputDestMAC, unsigned char *inputFrameType)
{
  static int _crcTableData[16] = {
      0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
      0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
      0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
      0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };
  
  _crc_table = &_crcTableData[0];

  _datalength = inputDatalength;
  _framesize = 14 + _datalength + 4;
  _incomingByte = 0;
  _sourceMAC = inputSourceMAC;
  _destMAC = inputDestMAC;
  _frameType = inputFrameType;

  MANCHESTER.SetTxPin(4);
  Serial.begin(9600);

  int i;
  char payload[_datalength+1];
  char appended[_framesize];
  data_generation(payload,_datalength);
  
  Serial.println("Payload:");
  print_frame(payload,_datalength);
 
  append_all(appended,payload, _framesize);
  
  long crc = crc_string(appended);
  
  Serial.println("Full Frame:");
  print_frame(appended, _framesize);
  
  Serial.println("CRC:");
  Serial.println(crc,HEX);  
}

unsigned long LinkLayer::crc_update(unsigned long crc, byte data){
	byte tbl_idx;
    tbl_idx = crc ^ (data >> (0 * 4));
    crc = pgm_read_dword_near(_crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);
    tbl_idx = crc ^ (data >> (1 * 4));
    crc = pgm_read_dword_near(_crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);
    return crc;
}

unsigned long LinkLayer::crc_string(char *s)
{
  unsigned long crc = ~0L;
  while (*s)
    crc = crc_update(crc, *s++);
  crc = ~crc;
  return crc;
}

void LinkLayer::data_generation(char *payload, int inputDataLength)
{  
  int i;
  for(i=0;i<inputDataLength;i++)
    payload[i] = random(10)+0x41;
  payload[i]='\0';  
}

void LinkLayer::print_frame(char *stuff, int framesize)
{
  int i;
  for(i=0; i<framesize; i++){
    if (stuff[i]<0x10)
      Serial.print("0");
    Serial.print(stuff[i],HEX);
  }
  Serial.println();
  Serial.println();
}

void LinkLayer::append_all(char *all_appended, char *payload, int framesize){
  int i, j, k, z, y;
  
  for(i=0; i<framesize; i++) all_appended[i]=0;
  
  for(i=0; i<6; i++) all_appended[i] = _destMAC[i];
  //print_frame(all_appended, framesize);
  

  for(j=0; j<6; j++) all_appended[j+6] = _sourceMAC[j];
  //print_frame(all_appended, framesize);
  

  for(z=0; z<2; z++) all_appended[z+12] = _frameType[z];  
  //print_frame(all_appended, framesize);
  

  for(k=0; k<_datalength+1; k++) all_appended[k+14] = payload[k];
  
  all_appended[framesize-4]='\0';
  
  
  Serial.println("Frame:");
  print_frame(all_appended, framesize);
  
  long crc = crc_string(all_appended);
  
  Serial.println("CRC:");
  Serial.println(crc,HEX);
  
  for (y=3; y>=0; y--)
  {
    Serial.println(crc>>(y*0x08)&0xFF,HEX);
    all_appended[framesize-y-1]=crc>>(y*0x08)&0xFF;
  }
  
  all_appended[framesize]='\0';
  //print_frame(all_appended, framesize);
}
