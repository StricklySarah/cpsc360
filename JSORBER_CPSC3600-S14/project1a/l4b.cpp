//Sarah Strickland

#include <assert.h>
#include <string.h>
#include "logger.h"

#include "l3.h"
#include "l4b.h"



int L4b::init(L4b_upper *upper, L3 *lower){
	
	m_upper = upper;
	m_lower = lower;

	return 1;
}


void L4b::pop(uint8_t *message, int length){

	uint16_t listlen;
	uint16_t *command_lens;
	uint16_t **commands;
	int size = 0;
	int size2 = 0;
	
	memcpy((void*)&listlen,message,sizeof(uint16_t));	

	command_lens = (uint16_t*) malloc(sizeof(listlen));

	for(int i=0; i < listlen; i++){
		memcpy((void *)&command_lens[i], message+(sizeof(listlen))+(2*i),sizeof(uint16_t));
		size++;
	}	

	commands = (uint16_t**) malloc(sizeof(listlen));

	for(int i= 0; i < listlen; i++){
		commands[i] = (uint16_t*)malloc(sizeof(command_lens[i]));
	}

	for(int i = 0; i <listlen; i++){
		memcpy((void *)&commands[i], message+sizeof(listlen)+(2*size)+size2, command_lens[i]);
		size2+=command_lens[i];
	} 

	listlen = (int)listlen;
	
	m_upper->pop((uint8_t**)commands,(int*)command_lens, listlen);

	free(commands);
	free(command_lens);

}	


int L4b::push(uint8_t **commands, int *command_lengths, int command_list_length){

	uint16_t total_len = 0;
	uint16_t listlen = command_list_length;
	uint16_t * command_lens = (uint16_t *)malloc(2*listlen);
	uint16_t space_used = 0;

	for(int i = 0; i< listlen; i++){
		command_lens[i] = (uint16_t)command_lengths[i];
	}	

	for(int i=0; i < listlen; i++){
		total_len = total_len + command_lens[i];
	}

	int message_size = (sizeof(listlen)+(2*listlen)+total_len);
	output(LOG_L4B, "Message Size: %d", message_size);
	
	uint8_t *message = (uint8_t *) malloc (message_size);

	//copy the number of commands into the message
	memcpy(message, &listlen, sizeof(listlen));

	//go through the number of commands
	for(int i= 0; i < listlen; i++){
		memcpy(message+sizeof(listlen)+space_used,&command_lens[i],2);
		space_used+=2;
	}
	for(int i = 0; i < listlen; i++){
		memcpy(message+sizeof(listlen)+space_used,commands[i],command_lens[i]);
		space_used+=command_lens[i];	
	}

	m_lower->push(message,space_used+2);

	free(message);
	free(command_lens);

	return command_list_length;	
}




