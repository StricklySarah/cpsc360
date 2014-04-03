#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#pragma pack(1)
#define PACKET_SIZE 7


typedef struct teststruct {
	short a;
	short b;
	uint8_t op;
	uint16_t other;
} ts;

#define OFFSET_B sizeof(short)
#define OFFSET_OP OFFSET_B + sizeof(short)
#define OFFSET_OTHER OFFSET_OP + sizeof(uint8_t)


int main()
{
	ts t;
	uint8_t *p;
	
	unsigned char *buffer  = (unsigned char*)malloc(PACKET_SIZE);
	
	t.a = 0xDEAD;
	t.b = 0xBEEF;
	t.op = 8;
	t.other = 0xFEED;
	
	p = (uint8_t*)&t;
	
	for (int i=0; i < sizeof(t); i++)
		{   printf("%X ",p[i]);  } //p[i] == *(p+i)
	printf("\n");
	
	t.a = htons(0xDEAD);
	t.b = htons(0xBEEF);
	t.op = 8;
	t.other = htons(0xFEED);
	
	
	bzero(buffer, PACKETSIZE);
	memset(buffer, 0, PACKET_SIZE);
	
	memcpy(buffer, &t.a, sizeof(t.a));
	memcpy(buffer+OFFSET_B, &t.b, sizeof(t.b));
	memcpy(buffer+OFFSET_OP, &t.op, sizeof(t.op));
	memcpy(buffer+OFFSET_OTHER, &t.other, sizeof(t.other));
	
	
	
	for (int i=0; i < PACKET_SIZE; i++)
		{   printf("%X ",buffer[i]);  } 
	
	printf("\n");
	
	free(buffer); //clean up after ourselves
	buffer = NULL;
}