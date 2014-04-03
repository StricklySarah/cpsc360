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

#include <unistd.h>
#include <getopt.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void fheader(uint8_t* buff);

int fquery(uint8_t* buff, char* name);

int* parseAnswerHeader(uint8_t* answer, uint8_t* anspt);

void parseName(uint8_t* buff, uint8_t* startRead, uint8_t* readInto, int startWrite);

int* parseAnswerData(uint8_t* answerStart);

int main(int argc, char **argv);

#endif
