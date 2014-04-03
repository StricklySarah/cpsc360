#ifndef _DNSClient_H_
#define _DNSClient_H_

#define MAX_16_BIT 65535

#include <unistd.h>
#include <getopt.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void fheader(uint8_t* buff);

void fquery(uint8_t* buff, char* name);

int createSend(struct sockaddr_in addr, char*buff, char*ip, char* port);

int main(int argc, char **argv);

#endif
