//#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "logger.h"
#include "l2.h"
#include "l3.h"


int L3::init(L3_upper *upper, L2 *lower)
{
	m_upper = upper;
	m_lower = lower;
	
	return 1;
}


void L3::pop(uint8_t *message, int length)
{
	
	assert(message != NULL);
	assert(length >= (int) sizeof(uint32_t));
	
	
	output(LOG_L3, "Received a message (%d bytes)",length);
	uint32_t yourchecksum;
	uint32_t mychecksum = 0;
	
	memcpy((uint8_t*)&yourchecksum, message, sizeof(uint32_t));
	for (int i=sizeof(uint32_t); i < length; i++)
	{
		mychecksum += message[i];
	}
	
	if (yourchecksum == mychecksum)
	{
		output(LOG_L3, "Checksums Matched! (%d == %d)",yourchecksum, mychecksum);
		m_upper->pop(message+sizeof(uint32_t), length-sizeof(uint32_t));
	} else {
		output(LOG_L3, "Checksums don't matched. :( (%d != %d)",yourchecksum, mychecksum);
		//don't pop, since the data is messed up.
	}
	
	
	
}	


//send a chunk
int L3::push(uint8_t *message, int length)
{

	if (message == NULL) 
	{
		output(LOG_L3,"ERROR!! PUSHED (from %X) a NULL message", this);
		return -1;
	}
	
	output(LOG_L3,"PUSH (from %X), %i bytes", this, length);

	uint8_t *new_message = (uint8_t*)malloc(length+4);
	
	//compute checksum over all bytes
	uint32_t chksum = 0;
	for (int i=0; i < length; i++)
	{
		chksum += message[i];
	}
	output(LOG_L3,"Checksum = %d", chksum);
	
	memcpy(new_message, (uint8_t*)&chksum, sizeof(chksum));
	memcpy(new_message+sizeof(chksum),message,length);
	
	m_lower->push(new_message, length+sizeof(chksum));
	
	free(new_message);
	
	
	output(LOG_L3,"PUSH (from %X) finished! (%d)", this);
	return length;
}
