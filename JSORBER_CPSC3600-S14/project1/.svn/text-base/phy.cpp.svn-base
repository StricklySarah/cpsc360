#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "phy.h"
#include "logger.h"

//------------------------------------------------------------------
// Sample PHY layer implementation, that simulates the wire.
//

//#define LOSSY //uncomment this to have the phy layer occasionally lose bytes
#define LOSSY_PERCENT 5 //the percentage of bytes that get lost

//#define NOISY //uncomment this to have the phy layer occasionally corrupt bytes
#define NOISY_PERCENT 5 //the percentage of bytes that get corrupted

//useful for making losses deterministic, comment this out for less deterministic behavior
//     change the value for different deterministic behavior
#define PHY_RANDOM_SEED 5 





PHY_upper* PHY::PHY_Clients[MAX_CLIENTS];

//initialize to the phy layer
int PHY::init(PHY_upper *up) {
	
	m_upper = up;
	m_total_bytes = 0;
	
	for (int i=0; i < MAX_CLIENTS; i++)
	{
		if (PHY_Clients[i] == NULL)
		{
			PHY_Clients[i] = m_upper;
			m_phy_id= i;
			output(LOG_PHY, "Connected client #%d",i);
			return i;
		}
	}
	output(LOG_PHY, "Too many clients.");
	return -1;
}

uint8_t prob_corrupt_byte(uint8_t thebyte)
{
#ifdef NOISY
	if ((rand() % 100) < NOISY_PERCENT)
	{
		//flip a random bit
		output(LOG_PHY, "Byte corrupted.");
		return (thebyte ^ (1 << (rand() % 7)));
	}
#endif
	return thebyte;
}

void PHY::push(uint8_t the_byte)
{
	//simulate putting a byte on the wire
	//may lose or corrupt bytes here.
	m_total_bytes++;
	
	for (int i=0; i < MAX_CLIENTS; i++)
	{
		if (PHY_Clients[i] != m_upper)
		{
			//printf("\t(%i,%i,%i)",i,client_id,phy_clients[i].valid);
#ifdef LOSSY
			if ((rand() % 100) < LOSSY_PERCENT)
			{
				output(LOG_PHY, "Byte lost (from %i, to %i)",m_phy_id,i);
				continue;
			}
#endif			
			PHY_Clients[i]->pop(prob_corrupt_byte(the_byte));
		}
	}
}

int PHY::total_bytes_sent()
{
	return m_total_bytes;
}

