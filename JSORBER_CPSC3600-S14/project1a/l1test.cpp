#include <stdint.h>
#include <string.h>
#include "logger.h"
#include "l1.h"

/****
Sample program that directly tests the first layer
*****/

class TestClient : public L1_upper
{
	
public:
	int id;
	void pop(uint8_t *chunk, int length);
};



void TestClient::pop(uint8_t *chunk, int length)
{
	char buffer[100];
	memset(buffer, 0, 100);
	memcpy(buffer,chunk,length);
	
	
	output(LOG_CLIENT, "client #%i received chunk: %s (%i bytes)",id,buffer,length);
}




int main(int argc, char *argv[])
{
	
	TestClient t1,t2;
	PHY p1,p2;
	L1 l1,l2;
	
	uint8_t msg1[11] = "JacobSorbe";
	uint8_t msg2[7] = "Sorber"; 
	
	
	//initialize PHY first
	p1.init(&l1);
	p2.init(&l2);
	
	//initialize L1
	t1.id = l1.init(&t1,&p1);
	t2.id = l2.init(&t2,&p2);
	
	
	l1.push(msg1,10);
	l2.push(msg2,6);
	
	
}
