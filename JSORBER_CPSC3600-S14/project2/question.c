#include "DNSClient.h"


char* makeQuestion(char* name){
	char* question = NULL;
	int index = 0;
	int length = strlen(name);
	char *dot;

	question = (char*) malloc(sizeof(name)+16);
	
	dot=strchar(name, '.');

	while(dot != NULL){
		
		dot=strchar(name, '.');
	}

	//QNAME
	question[index] = 3;
	index++;
	question[index] = 
	//QTYPE use 0x0001
	question[index] = ;
	//QClass use 0x0001
	
}
