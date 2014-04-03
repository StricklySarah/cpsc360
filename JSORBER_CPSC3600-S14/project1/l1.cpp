//#include <stdio.h>
#include <assert.h>
#include "logger.h"
#include "phy.h"
#include "l1.h"





int L1::init(L1_upper *upper, PHY *lower)
{
	m_upper = upper;
	m_lower = lower;
	
	m_index = 0;
	
	if (upper == NULL || lower == NULL) return -1;
	else return 1;
}

//an implementation of L1 that only works when PHY isn't lossy or noisy
void L1::pop(uint8_t b)
{
	output(LOG_L1,"Rx (x%X) (%d,%d)",b,m_current_chunk[0],m_index);
	
//	assert(m_index != 0 || (b >= 0 || b < L1_CHUNK_MAX_SIZE));
	
	m_current_chunk[m_index] = b;
	if (m_index == m_current_chunk[0]) 
	{
		output(LOG_L1,"Popping %d",m_current_chunk[0]);
		m_upper->pop(m_current_chunk+1,m_current_chunk[0]);
		m_index = 0;
		return;
	}
	
	if (m_index < L1_CHUNK_MAX_SIZE) m_index++;
}	


//send a chunk
int L1::push(uint8_t *chunk, int length)
{
	output(LOG_L1,"Sending (from %X), %i bytes", this, length);
	
	if (length > L1_CHUNK_MAX_SIZE) return -1;
	
	m_lower->push(0xFF & length);
	for (int i=0; i < length; i++)
	{
		m_lower->push(chunk[i]);
	}
	return length;
}

