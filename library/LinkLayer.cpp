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
  
  _crc_table = _crcTableData;

  _datalength = inputDatalength;
  _framesize = 14 + _datalength + 4;
  _incomingByte = 0;
  _sourceMAC = inputSourceMAC;
  _destMAC = inputDestMAC;
  _frameType = inputFrameType;
  
  _max_bytes = 100; 
  
  unsigned char rec_packetLocal[100];
  _rec_packet = rec_packetLocal; 
  char rec_packet_charLocal[100];
  _rec_packet_char = rec_packet_charLocal;  

  MANCHESTER.SetTxPin(4);
  Serial.begin(9600); 
}

/* ---------------------------------------- TOP LAYER FUNCTIONS ------------------------------------------*/

void LinkLayer::send_message(){
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

void LinkLayer::receive_message(){
  Serial.begin(9600);
  // Set digital TX pin
  MANRX_SetRxPin(4);
  // Prepare interrupts
  MANRX_SetupReceive();
  // Begin receiving data
  MANRX_BeginReceiveBytes(_max_bytes,_rec_packet);

  
  // checkMAC example
  if (1==0) {
    char MAC_example[7] = "nathan";
    int result;
    result = checkMAC(MAC_example);
    Serial.println(result);
  }
  
  //checkFrametype example
  if (1==0) {
    int i, j, z, k, y;
    int framesize = 28;
    int datalength = 10;
    char frame_example[framesize];
    char exdestMAC[7] = "nathan";
    char exsourceMAC[7] = "allenk";
    char expayload[11] = "thisisdata";
    char excrc[5] = {0xD1, 0x52, 0x67, 0xB4};
    unsigned char exframeType[3] = {0x08, 0x10, '\0'};
    for(i=0; i<framesize; i++) frame_example[i]=0;
    for(i=0; i<6; i++) frame_example[i] = exdestMAC[i];
    for(j=0; j<6; j++) frame_example[j+6] = exsourceMAC[j];
    for(z=0; z<2; z++) frame_example[z+12] = exframeType[z];  
    for(k=0; k<datalength+1; k++) frame_example[k+14] = expayload[k];
    for(y=0; y<4; y++) frame_example[y+24] = excrc[y];
    frame_example[framesize]='\0';
//    print_frame((unsigned char *)frame_example, framesize);
    int result;
    result = checkFrametype(frame_example);
    Serial.println(result);
  }
  
  //checkCRC example
  if (1==1) {
    int i, j, z, k, y;
    int framesize = 28;
    int datalength = 10;
    char crc_example[framesize];
    char exdestMAC[7] = "nathan";
    char exsourceMAC[7] = "allenk";
    char expayload[11] = "thisisdata";
    char excrc[5] = {0xD1, 0x52, 0x67, 0xB4};
    unsigned char exframeType[3] = {0x08, 0x10, '\0'};
    for(i=0; i<framesize; i++) crc_example[i]=0;
    for(i=0; i<6; i++) crc_example[i] = exdestMAC[i];
    for(j=0; j<6; j++) crc_example[j+6] = exsourceMAC[j];
    for(z=0; z<2; z++) crc_example[z+12] = exframeType[z];
    for(k=0; k<datalength+1; k++) crc_example[k+14] = expayload[k];
    for(y=0; y<4; y++) crc_example[y+24] = excrc[y];
    crc_example[framesize]='\0';
    print_frame(crc_example,framesize);
    int result;
    result = checkCRC(crc_example);
    Serial.println(result);
  }
  
}

/* ---------------------------------------- SEND HELPER FUNCTIONS ------------------------------------------*/

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

/* ---------------------------------------- RECEIVE HELPER FUNCTIONS ------------------------------------------*/

int LinkLayer::checkMAC(char *received){
  char myMAC[7] = "nathan";
  int i;
  for (i=0; i<7; i++) {
    if (myMAC[i] != received[i])
      return 1;
  }
  Serial.println("checkMAC worked");
  return 0;
}


int LinkLayer::checkFrametype(char *received){
  unsigned char myFrame[3] = {0x08, 0x10, '/0'};
//  print_frame(received, 18);
  int i;
  for (i=0; i<2; i++) {
    Serial.println((unsigned char) received[i+12], HEX);
    if (myFrame[i] != received[i+12])
      return 1;
  }
  Serial.println("checkFrametype worked");
  return 0;
}

int LinkLayer::checkCRC(char *received){
  char* substr = "";
  long rec_crc = 0;
  long temp;
  int len = strlen(received);
  int y;
  substr = strncpy(substr, received, len-4);
  substr[len-4] = '\0';
  
  print_frame(received, len);
  print_frame(substr, len-4);

  for (y=3; y>=0; y--)
  {
    temp = received[len-y-1];
    Serial.println(temp, HEX);
    Serial.println(temp<<(y*0x08), HEX);
    rec_crc += temp<<(y*0x08);
  }  
  Serial.println(rec_crc, HEX);

  long check_crc = crc_string(substr);
  Serial.println(check_crc, HEX);
  if (check_crc == rec_crc){
    Serial.println("checkCRC worked");
    return 0;
  }
  return 1;
}

