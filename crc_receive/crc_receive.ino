#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MANCHESTER.h>

unsigned char max_bytes = 100;
unsigned char rec_packet[100];
char rec_packet_char[100];
unsigned char *rec_data;

void print_frame(unsigned char *stuff, int framesize)
{
  int i;
  for(i=0; i<framesize; i++){
    if (stuff[i]<0x10)
      Serial.print("0");
    Serial.print(stuff[i], HEX);
  }
  Serial.println();
  Serial.println();
}

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

int checkMAC(char *received){
  char myMAC[7] = "nathan";
  int i;
  for (i=0; i<7; i++) {
    if (myMAC[i] != received[i])
      return 1;
  }
  Serial.println("checkMAC worked");
  return 0;
}

int checkFrametype(char *received){
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

int checkCRC(char *received){
  char* substr = "";
  long rec_crc = 0;
  long temp;
  int len = strlen(received);
  int y;
  substr = strncpy(substr, received, len-4);
  substr[len-4] = '\0';
  
  print_frame((unsigned char *) received, len);
  print_frame((unsigned char *) substr, len-4);

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
  
  
void setup()
{  
  Serial.begin(9600);
  // Set digital TX pin
  MANRX_SetRxPin(4);
  // Prepare interrupts
  MANRX_SetupReceive();
  // Begin receiving data
  MANRX_BeginReceiveBytes(max_bytes,rec_packet);

  
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
    print_frame((unsigned char *)crc_example,framesize);
    int result;
    result = checkCRC(crc_example);
    Serial.println(result);
  }
  
}

void loop() {
  if (1==0){
    //print_frame(rec_packet, 38);
    int i;
    for (i=0; i<100; i++) {
      rec_packet_char[i] = (char) rec_packet[i];
    }
    
    if (MANRX_ReceiveComplete()) {
      // Handle data...
      Serial.println("IT'S HERE");
      if (!checkMAC(rec_packet_char) && !checkFrametype(rec_packet_char)) {
        if (!checkCRC(rec_packet_char)) {
          Serial.println("IT WORKED");
        }
      } 
    }
    
  }
}
