//#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "logger.h"
#include "l3.h"
#include "l4.h"


char *make_hex(char *buf, int length)
{
	char *result = (char*)malloc(length*3);
	char stbuf[20];
	
	result[0] = 0;
	for (int i=0; i < length; i++)
	{
		sprintf(stbuf,"%X ",buf[i]);
		strcat(result, stbuf);
	}
	return result;
}

int L4::init(L4_upper *upper, L3 *lower)
{
	assert(upper != NULL);
	assert(lower != NULL);
	
	m_upper = upper;
	m_lower = lower;
	
	return 1;
}


void L4::pop(uint8_t *message, int length)
{
	
	assert(message != NULL);
	assert(length >= (int)(2*sizeof(uint32_t)));
	
	output(LOG_L4, "Received a message %.s",length, message);
	
	char* tmp = make_hex((char*)message, length);
	output(LOG_L4, "Received a message %s",tmp);
	free(tmp);
	
	uint32_t nlen, vlen;
	
	memcpy((void*)&nlen, message, sizeof(nlen));
	memcpy((void*)&vlen, message+sizeof(nlen), sizeof(vlen));
	uint8_t *thename = message +sizeof(nlen)+sizeof(vlen);
	uint8_t *thevalue = thename+nlen;
	
	tmp = make_hex((char*)thename, nlen);
	output(LOG_L4, "NAME: %s",tmp);
	free(tmp);
	
	tmp = make_hex((char*)thevalue, nlen);
	output(LOG_L4, "VALUE: %s",tmp);
	free(tmp);
	
	
	output(LOG_L4, "Popping %d,%d",nlen,vlen);
	output(LOG_L4, "Popping %.*s,%.*s",nlen,thename,vlen,thevalue);
	//output(LOG_L4, "Popping %.s(%d),%.s(%d)",nlen,thename, nlen, vlen, thevalue, vlen);
	m_upper->pop(thename, nlen, thevalue, vlen);
	
}	




//send a named value
int L4::push(uint8_t *name, int namelength, uint8_t *value, int valuelength)
{

	assert(name != NULL);
	assert(value != NULL);
	
	uint32_t nlen, vlen;
	nlen = namelength;
	vlen = valuelength;
	
	
	output(LOG_L4,"PUSH (from %X), %.*s : %.*s", this, namelength, name, valuelength, value);

	int message_size = sizeof(nlen)+sizeof(vlen) + namelength + valuelength;
	uint8_t *new_message = (uint8_t*)malloc(message_size);
	
	memcpy(new_message, (uint8_t*)&nlen, sizeof(nlen));
	memcpy(new_message+sizeof(nlen), (uint8_t*)&vlen, sizeof(vlen));
	memcpy(new_message+sizeof(nlen)+sizeof(vlen), name, nlen);
	memcpy(new_message+sizeof(nlen)+sizeof(vlen)+nlen, value, vlen);

	
	m_lower->push(new_message, message_size);
	
	free(new_message);
	
	
	output(LOG_L4,"PUSH (from %X) finished!", this);
	return message_size;
}
