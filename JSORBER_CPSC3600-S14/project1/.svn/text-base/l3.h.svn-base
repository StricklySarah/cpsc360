
#ifndef _L3_H_
#define _L3_H_

#include "l2.h"

//specifies the interface for the upper layer (DON'T CHANGE THIS)
class L3_upper {
public:
	virtual void pop(uint8_t *message, int length) = 0;
};


//The implementation of your Layer 3
class L3 : public L2_upper {
	L3_upper* m_upper;
	L2 *m_lower;
	
public:
	
	int init(L3_upper *upper, L2* lower);
	
	//the method the upper layer calls to send a message 
	//by making calls to L2
	int push(uint8_t *message, int length);
	
	//the method that gets called by L2 when a message is received
	void pop(uint8_t *message, int length);
};


#endif //_L3_H_