#ifndef _L1_H_
#define _L1_H_

#define L1_CHUNK_MAX_SIZE 10

#include "phy.h"
#include <stdlib.h>

//specifies the interface for the upper layer (DON'T CHANGE THIS)
class L1_upper {
public:
	virtual void pop(uint8_t *chunk, int length) = 0;
};




//The implementation of your Layer 1
class L1 : public PHY_upper {
	L1_upper* m_upper;
	PHY *m_lower;
	
	uint8_t m_current_chunk[L1_CHUNK_MAX_SIZE];
	int m_index;

public:
	//initialize the layer
	int init(L1_upper *upper, PHY* lower);
	
	//the method the upper layer calls to send a chunk
	int push(uint8_t *chunk, int length);
	
	//the method that gets called by the phy layer when a byte is received
	void pop(uint8_t b);
};




#endif

