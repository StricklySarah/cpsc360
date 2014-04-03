//Sarah Strickland

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
	//check that message isnt NULL and the legth is corrent 
	assert(message != NULL); 
	
	output(LOG_L3, "Received a message (%d bytes)",length);


	uint32_t given_checksum;
	uint32_t computed_checksum = 0;
	
	//remove check sum that was sent in
	memcpy((uint8_t*)&given_checksum, message, sizeof(uint32_t));

	//compute current check sum
	for (int i=sizeof(uint32_t); i < length; i++){
		computed_checksum += message[i];
	}
	
	//check to see if check sums matched
	//if not corrupted pop the messaged
	if (given_checksum == computed_checksum){
		output(LOG_L3, "Checksums Matched! (%d == %d)",given_checksum, computed_checksum);
		m_upper->pop(message+sizeof(uint32_t), length-sizeof(uint32_t));
	}
	//else don't do anything 
	else {
		output(LOG_L3, "Checksums don't matched. :( (%d != %d)",given_checksum, computed_checksum);
		//don't pop, since the data is messed up.
	}
	
	
	
}	


//send a chunk
int L3::push(uint8_t *message, int length)
{
	//check if message is null
	if(message == NULL){
		output(LOG_L3, "message NULL, not pushed");
		return -1;	
	}
	
	output(LOG_L3,"PUSH (from %X), %i bytes", this, length);

	//create an array to hold the check sum and the message
	uint8_t *check_message = (uint8_t*)malloc(length+4);
	
	//compute checksum
	uint32_t checksum = 0;
	for (int i=0; i < length; i++){
		checksum += message[i];
	}
	output(LOG_L3,"Checksum = %d", checksum);
	
	//put check sum at the begginning of the message
	memcpy(check_message, (uint8_t*)&checksum, sizeof(checksum));

	//add the rest of the message after the cheeck sum
	memcpy(check_message+sizeof(checksum),message,length);
	
	//push the message with the check sum
	m_lower->push(check_message, length+sizeof(checksum));
	
	//free memeory 
	free(check_message);
	
	
	output(LOG_L3,"PUSH (from %X) finished! (%d)", this);
	return length;
}

