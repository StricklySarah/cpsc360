#ifndef _PHY_H_
#define _PHY_H_

#define TRUE 1
#define FALSE 0
#define MAX_CLIENTS 2

#include <stdio.h>
#include <stdint.h>

//the interface to the upper layer (DON'T CHANGE IT!)
class PHY_upper {
public:
	virtual void pop(uint8_t b) = 0;
};


//IMPLEMENTATION


//The implementation of the PHY layer
class PHY {
	static PHY_upper* PHY_Clients[MAX_CLIENTS];
	int m_phy_id;
	int m_total_bytes;
	PHY_upper* m_upper;
	
	//void global_phy_init();
	
public:

	//connect to the physical medium "the wire"
	//tell it what function to call when a byte is received
	int init(PHY_upper *upper);
	
	//put a byte on the wire
	void push(uint8_t the_byte);
	
	//used to determine how efficient your protocols are.
	int total_bytes_sent();
};

#endif

