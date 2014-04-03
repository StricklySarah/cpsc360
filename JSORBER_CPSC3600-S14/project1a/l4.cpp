//Sarah Strickland

#include <assert.h>
#include <string.h>
#include "logger.h"
#include "l3.h"
#include "l4.h"



int L4::init(L4_upper *upper, L3 *lower){
	m_lower = lower;
	m_upper = upper;

	return 1;
}


void L4::pop(uint8_t *message, int length){

	assert(message != NULL);

	assert(length >= (int)(2*sizeof(uint32_t)));

	output(LOG_L4, "Received: %s of length %i", message, length);

	uint8_t  valuelen, namelen;
	uint8_t *name, *value;

	memcpy((void*)&namelen, message, sizeof(namelen));
	memcpy((void*)&valuelen, message+sizeof(namelen)+namelen, sizeof(valuelen));

	name = message + sizeof(namelen);
	value = name + namelen + 1;

	m_upper->pop(name, namelen, value, valuelen);

}	

int L4::push(uint8_t *name, int namelength, uint8_t *value, int valuelength){
	
	assert(name != NULL || value != NULL );

	uint8_t namelen, valuelen;
	
	namelen= namelength;
	valuelen = valuelength;

	int totalsize = namelen + valuelen + sizeof(namelen) + sizeof(valuelen);

	uint8_t *complied_message = (uint8_t*)malloc(totalsize);

	memcpy(complied_message, (uint8_t*)&namelen, sizeof(namelen));
	memcpy(complied_message + sizeof(namelen), name, namelen);
	memcpy(complied_message + sizeof(namelen) + namelen, (uint8_t*)&valuelen, sizeof(valuelen));
	memcpy(complied_message + sizeof(namelen) + namelen + sizeof(valuelen), value, valuelen);

	output(LOG_L4, "Pushed Message(%i, %s, %i, %s)",namelen,name,valuelen,value); 

	m_lower->push(complied_message, totalsize);

	free(complied_message);

	return totalsize;

	
}
