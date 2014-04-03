#ifndef _L2_H_
#define _L2_H_

#include "l1.h"

//specifies the interface for the upper layer (DON'T CHANGE THIS)
class L2_upper {
public:
	virtual void pop(uint8_t *message, int length) = 0;
};


//The implementation of your Layer 2
class L2 : public L1_upper {
	L2_upper* m_upper;
	L1 *m_lower;
	
	uint8_t *rx_current_message;
	int rx_chunk_count;
	int chunk_count;
	int tx_done;
	
public:
	
	int init(L2_upper *upper, L1* lower);
	
	//the method the upper layer calls to send a chunk
	int push(uint8_t *message, int length);
	
	//the method that gets called by L1 when a chunk is received
	void pop(uint8_t *chunk, int length);
};


#endif //_L2_H_