//Sarah Strickland

#ifndef _DNSClient_H_
#define _DNSClient_H_

#define MAX_16_BIT 65535
#define QR     0b1000000000000000
#define OPCODE 0b0111100000000000
#define AA     0b0000010000000000
#define TC     0b0000001000000000
#define RD     0b0000000100000000
#define RA     0b0000000010000000
#define Z      0b0000000001110000
#define RCODE  0b0000000000001111
#define OFFSET_MASK 0b001111111111111
////             10000010011111110100010101001011
#define BYTE1  0b11111111000000000000000000000000
#define BYTE2  0b00000000111111110000000000000000
#define BYTE3  0b00000000000000001111111100000000
#define BYTE4  0b00000000000000000000000011111111


#include <unistd.h>
#include <getopt.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void fheader(uint8_t* buff);

int fquery(uint8_t* buff, char* name);

int* parseAnswerHeader(uint8_t* answer, uint8_t* anspt, int* returnThis);

void parseName(uint8_t* buff, uint8_t* startRead, uint8_t* readInto, int startWrite);

int* parseAnswerData(uint8_t* answerStart, int* returnMe);

int main(int argc, char **argv);

#endif
