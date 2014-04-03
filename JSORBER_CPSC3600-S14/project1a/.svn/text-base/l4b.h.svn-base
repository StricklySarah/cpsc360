#ifndef _L4B_H_
#define _L4B_H_

#include "l3.h"

//specifies the interface for the upper layer (DON'T CHANGE THIS)
class L4b_upper {
public:
	virtual void pop(uint8_t **commands, int *command_lengths, int command_list_length) = 0;
};


//The implementation of your Layer 4b
//YOU IMPLEMENT THIS

class L4b : public L3_upper {
	L4b_upper* m_upper;
	L3 *m_lower;
	
public:
	
	int init(L4b_upper *upper, L3* lower);
	
	//the method the upper layer calls to send a message 
	//by making calls to L3
	int push(uint8_t **commands, int *command_lengths, int command_list_length);
	
	//the method that gets called by L3 when a message is received
	void pop(uint8_t *message, int length);
	
};


#endif //_L4_H_