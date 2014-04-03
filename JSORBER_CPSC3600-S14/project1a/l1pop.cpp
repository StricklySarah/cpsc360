void L1::pop(uint8_t b) 
{
	if (m_index == 0){
		output(LOG_L1,"Rx (%i) (%d,%d)",b,(m_current_chunk[0]),m_index);	
	}else{	
		output(LOG_L1,"Rx (%c) (%d,%d)",b,(m_current_chunk[0]),m_index);
	}

	output(LOG_L1, "frame_detection: %i", m_frame_detection);
	
	// if idex=0 and not frame, return
	if(b != FRONTFRAME && m_index == 0 && m_frame_detection == false){
		output(LOG_L1, "Didn't start with a framing character, trashing chunk");
		m_index = 0;
		return;
	}
	// if first IS frame
	if(b == FRONTFRAME) {
		if(m_index == 0){
			m_frame_detection = true;
		} else {	
			output(LOG_L1, "Bytes lost, trashing chunk");
			m_frame_detection = false;
			m_index = 0;
			return;	
		}
	}
	// if length
	else if(b < 11 && m_index == 0 && m_frame_detection == true){
		m_current_chunk[m_index] = b;
	output(LOG_L1, "inc m_index: %i", m_index);
		m_index++;
	// if length is invalid, return
	}else if(m_index == 0 && m_frame_detection == true && b > 10){
		output(LOG_L1, "Invaild chunk size, trashing chunk");
		m_frame_detection = false;
		m_index = 0;
		return;
	
	}
	//  if regular byte not at end
	else if(m_index !=0 && m_index < L1_CHUNK_MAX_SIZE && m_frame_detection == true){
		m_current_chunk[m_index] = b;
output(LOG_L1, "inc m_index: %i",m_index );
		m_index++;
	} 
	// at end, no loss, pop
	else if (m_index == m_current_chunk[0]){
		output(LOG_L1,"Popping %d",m_current_chunk[0]);
		m_upper->pop(m_current_chunk+1,(m_current_chunk[0]));
		m_frame_detection = false;
		m_index = 0;
		return;		
	}

/*	output(LOG_L1,"Rx (%c) (%d,%d)",b,(m_current_chunk[LENGTH_INDEX]-1),m_index);
	
	//assert(m_index == LENGTH_INDEX && (b < 0 || b > L1_CHUNK_MAX_SIZE));
	
	m_current_chunk[m_index] = b; //current chunk at current index is the bit send 

	if(m_current_chunk[LENGTH_INDEX] > 11){
		return; 
	}

	if (m_index == m_current_chunk[LENGTH_INDEX] && m_current_chunk[FRONT_FRAME_INDEX] == FRONTFRAME){
		output(LOG_L1,"Popping %d",m_current_chunk[LENGTH_INDEX]);
		m_upper->pop(m_current_chunk+2,(m_current_chunk[LENGTH_INDEX]-1));
		m_index = 0;
		return;
				
	}else if(m_index == m_current_chunk[LENGTH_INDEX] && m_current_chunk[FRONT_FRAME_INDEX] != FRONTFRAME){
		m_index = 0;
		return;
	}
	if (m_index < L1_CHUNK_MAX_SIZE) m_index++; //if we haven't reached the end of the chunk inc the index */
	
}	

