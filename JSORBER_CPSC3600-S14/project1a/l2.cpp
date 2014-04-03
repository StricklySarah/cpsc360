//Sarah Strickland

#include "l1.h"
#include "l2.h"

#include <assert.h>
#include <string.h>
#include "logger.h"
#include "phy.h"


#define TX_CHUNK   128 
#define RX_ACK     0  
#define L2_HEADER_SIZE 2
#define L2_MAX_PAYLOAD (L1_CHUNK_MAX_SIZE - L2_HEADER_SIZE)


int L2::init(L2_upper *upper, L1 *lower){
	m_upper = upper;
	m_lower = lower;
	
	rx_chunk_count = 0;
	rx_current_message = NULL;
	tx_done = FALSE;

	return 1;
}


void L2::pop(uint8_t *chunk, int length){

output(LOG_L2, "chunk: %s", chunk);

	//buffer to hold created message
	uint8_t buffer[L1_CHUNK_MAX_SIZE]; 
	//
	uint8_t ack_buffer[L1_CHUNK_MAX_SIZE];
	
	assert(chunk != NULL);
	assert(length >= L2_HEADER_SIZE); 
	
	bzero(buffer,sizeof(buffer)); // zero out buffer
	memcpy(buffer,chunk,length); //copy chunk into buffer
	output(LOG_L2,"Rx %s (%d)",buffer,length);
	
	//get headers
	int packet_type = 128 & buffer[0];
	int payload_length = (15 & buffer[0]);
	int packet_count = buffer[1];
	
	if (packet_type == TX_CHUNK){
			
		//if first packet
		if(packet_count == 0){

			if(rx_current_message != NULL){ //if there is a message in the current message
				output(LOG_L2, "Abandoning old message (%d chunks)",rx_chunk_count);
				free(rx_current_message); //get rid of the old message
				rx_current_message = NULL; //reset the current message to NULL
			}
			rx_chunk_count = 0; // first chunk of the message
			rx_current_message = (uint8_t*)malloc(payload_length); //make room for the new message
		}
		
		int old_size = rx_chunk_count * L2_MAX_PAYLOAD; //the size of the previous chunk
		int new_size = old_size + payload_length; //size of the current chunk + old chunk
		
		uint8_t *new_buffer = (uint8_t*) malloc(new_size); //create a new buffer to hold the message 
		
		//if not on the first chunk
		if(rx_chunk_count > 0){
			assert(rx_current_message != NULL); //make sure the message isnt NULL
			memcpy(new_buffer, rx_current_message, old_size); // copy the current chunk into its place in the new buffer 
			free(rx_current_message); //the current message has been added so it's no longer needed
		}
		
		rx_current_message = new_buffer; //reset the current buffer
		memcpy(rx_current_message+old_size, buffer+L2_HEADER_SIZE, payload_length); //copy the current message(buffer) into the correct spot in the current_message buffer
		rx_chunk_count++; //move to the next chunk in the message
		
		if(payload_length < L2_MAX_PAYLOAD){ //last chunk of the mesaage 
			m_upper->pop(rx_current_message, new_size); // pop the full message 
			free(rx_current_message); //release the message, no longer needed
			rx_current_message = NULL; //reset the current message for the next time a message is received
			rx_chunk_count = 0; //back to the first chunk because it will be a new message
		}	
			//create the acknowledgement buffer, which tells the sender the chunk was received 
			ack_buffer[0] = RX_ACK;
			ack_buffer[1] = packet_count;
			ack_buffer[2] = (rx_current_message == NULL); 
		output(LOG_L2, "pushing ack_buffer");
			m_lower->push(ack_buffer, sizeof(ack_buffer)); //send the acknowlegement to L1
	}		
	else if(packet_type == RX_ACK){ //message was received 
			
			output(LOG_L2, "Node %X received ACK (%d)",this, packet_count);
			
			if(packet_count == chunk_count){ //that was the last chunk in the message
				chunk_count++;
				if(buffer[2]) tx_done = TRUE; //we're done sending!
			}
	}
	else{
		output(LOG_L2, "ERROR, unknown packet type (%d)", packet_type);
	}
//	output(LOG_L2, "Packet_count: %i Chunk_count: %i", packet_count, chunk_count);
	
}	



int L2::push(uint8_t *message, int length){
	
	uint8_t chunk_buffer[L1_CHUNK_MAX_SIZE]; //holds chunk 
	
	output(LOG_L2,"PUSH (from %X), %i bytes", this, length);
	
	if(message == NULL) return -1;
	
	chunk_count = 0;
	tx_done = 0;  
	
	while(!tx_done){ //how do you know when you are done reading the message?
		
		int bytes_sent = (chunk_count *(L2_MAX_PAYLOAD));
		int bytes_to_send;
		int bytes_remaining =  (length - bytes_sent);
		
		if(bytes_remaining > L2_MAX_PAYLOAD)
			bytes_to_send = L2_MAX_PAYLOAD;
		else
			bytes_to_send = bytes_remaining; 
		
		//create header: change for more efficentcy  
		chunk_buffer[0] = TX_CHUNK | bytes_to_send;
		chunk_buffer[1] = chunk_count;  
		//middle 3 bits are unused 
		
		
		memcpy(chunk_buffer+L2_HEADER_SIZE/*copy into buffer_chunk after the header*/, 
			message+bytes_sent /*copy into buffer_chunk the next chunk that needs to be sent*/, 
			bytes_to_send /*length of the chuck being made*/);
	
		output(LOG_L2,"Sending (from %X), %i bytes (%d)", this, bytes_to_send, chunk_count);
		m_lower->push(chunk_buffer, L1_CHUNK_MAX_SIZE);	
	}

	output(LOG_L2,"PUSH (from %X) finished! (%d)", this, chunk_count);
	return length;
}	
