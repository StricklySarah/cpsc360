//Sarah Strickland

#include <assert.h>
#include "logger.h"
#include "phy.h"
#include "l1.h"

#define FRAME '/'
#define LENGTH_INDEX 1
#define FRONT_FRAME_INDEX 0


int L1::init(L1_upper *upper, PHY *lower)
{
	m_upper = upper;
	m_lower = lower;
	
	m_index = 0;
	
	
	if (upper == NULL || lower == NULL) return -1;
	else return 1;
}

//L1 recives bytes from sender by calling PHY pop()
//L1 sends data by using PHY push()
//L2 uses L1 push() to send chunks

void L1::pop(uint8_t b) 
{

	if(m_index != 0)
	output(LOG_L1,"Rx (x%c) (%d,%d)",b,m_current_chunk[0],m_index);
	else
	output(LOG_L1,"Rx (x%i) (%d,%d)",b,m_current_chunk[0],m_index);
	
	//if frame and length+1 equals m_index
	if(b == FRAME && (m_current_chunk[0]+1) == m_index){
		output(LOG_L1,"Popping %d",m_current_chunk[0]);
		m_upper->pop(m_current_chunk+1,m_current_chunk[0]);
		m_index = 0;
		return;
	}//at the end and not a frame return
	else if((m_current_chunk[0]+1) == m_index && b != FRAME){
		output(LOG_L1,"Lost Byte, trashing chunk 1");
		m_index = 0;
		return;
	}//if frame and not at the end return
	else if(b == FRAME && (m_current_chunk[0]+1) != m_index){
		output(LOG_L1, "Lost Byte, trashing chunk 2");
		m_index = 0;
		return;
	}//if not the frame add to m_current_chunk
	else if(b != FRAME && m_index <= L1_CHUNK_MAX_SIZE){
		m_current_chunk[m_index] = b;
		output(LOG_L1, "added b: %c to m_index: %i", b,m_index);
		m_index++;
		return;
	}
	output(LOG_L1, "Lost Frame, trashing chunk");
}	


int L1::push(uint8_t *chunk, int length)//called by higher levels to send chunks
//push sends down 
{
	output(LOG_L1, "Length: %i", length);
	
	if (length > L1_CHUNK_MAX_SIZE){ 
		output (LOG_L1, "length > L1_CHUNK_MAX_SIZE");
		return -1;
	}



	output(LOG_L1,"Sending (from %X), %i bytes", this, length);
	output(LOG_L1, "Pushing (%i)", (0xFF & length));
	
	m_lower->push(length); 

	for (int i=0; i < length; i++){
		m_lower->push(chunk[i]);
	}
	output(LOG_L1, "pushing(%i)", (0xFF & FRAME));
	m_lower->push(FRAME); //send frame

	return length;
}

