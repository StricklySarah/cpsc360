#ifndef _simhttp_H_
#define _simhttp_H_
 
#define SA  struct sockaddr
#define MAXLINE 4096
#define LISTENQ 1024
 
#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

 
int main(int argc, char **argv);
 
#endif

