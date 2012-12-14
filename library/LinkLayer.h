/*
  LinkLayer.h - Library for implementing Link Layer functionality on the Arduino.
  Created by Nathaniel Ting, Allen Koh, and Margaret-Ann Seger. December 12, 2012.
  Released into the public domain.
*/

#ifndef LinkLayer_h
#define LinkLayer_h

#include "MANCHESTER.h"
#include "Arduino.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>


class LinkLayer
{
  public:
  	// variables
    LinkLayer(int datalength, char *sourceMAC, char *destMAC, unsigned char *frameType);

    // overarching functions 
    void send_message();
    void receive_message();
    
    // send helper functions
    unsigned long crc_update(unsigned long crc, byte data);
    unsigned long crc_string(char *s);
    void data_generation(char *payload, int datalength);
    void print_frame(char *stuff, int framesize);
    void append_all(char *all_appended, char *payload, int framesize);
    
    // receive helper functions
    int checkMAC(char *received);
    int checkFrametype(char *received); 
    int checkCRC(char *received);
    
  private: 
    // send instance variables
    int _datalength;
    int _framesize;
    int _incomingByte;
    char *_sourceMAC;
    char *_destMAC;
    unsigned char *_frameType;
    int *_crc_table;
    
    // receive instance variables
    unsigned char _max_bytes; 
    unsigned char *_rec_packet;
    char *_rec_packet_char;
    unsigned char *_rec_data; 
};

#endif
