#include "logger.h"
#include "phy.h"

/****
Sample program that directly tests the physical layer
*****/

class TestClient : public PHY_upper
{
	
public:
	int id;
	virtual void pop(uint8_t abyte);
};

void TestClient::pop(uint8_t rxbyte)
{
	output(LOG_CLIENT, "client #%i received: %c(0x%X)",id, rxbyte,rxbyte);
}





int main(int argc, char *argv[])
{
	TestClient t1;
	TestClient t2;
	
	PHY p1;
	PHY p2;
	
	t1.id = p1.init(&t1);
	t2.id = p2.init(&t2);
	
	
	for (int i=0; i < 26; i++)
	{
		p1.push('a'+i);
	}
	for (int i=0; i < 26; i++)
	{
		p2.push('A'+i);
	}
}