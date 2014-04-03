#include <stdint.h>
#include <string.h>
#include "logger.h"
#include "l2.h"

/****
Sample program that directly tests the second layer
*****/

class TestClient : public L2_upper
{
	
public:
	int id;
	void pop(uint8_t *message, int length);
};


void TestClient::pop(uint8_t *message, int length)
{
	char msg[1000];
	memset(msg, 0, 1000);
	memcpy(msg,message,length);
	
	
	output(LOG_CLIENT, "client #%i received message: %s (%i bytes)",id, msg,length);
}




int main(int argc, char *argv[])
{
	
	TestClient t1,t2;
	PHY p1,p2;
	L1 l1a,l1b;
	L2 l2a,l2b;
	
	const char *msg1 = "To: Jacob Sorber. From: Someone else. Subject: This is a very pointless message. But it is long enough for a decent test.\n Sincerely, Myself.";
	const char *msg2 = "Thanks"; 
	
	
	//initialize PHY first
	p1.init(&l1a);
	p2.init(&l1b);
	
	//initialize L1
	l1a.init(&l2a,&p1);
	l1b.init(&l2b,&p2);
	
	//initialize l2
	t1.id = l2a.init(&t1,&l1a);
	t2.id = l2b.init(&t2,&l1b);
	
	l2a.push((uint8_t*)msg1,strlen(msg1)+1);
	l2b.push((uint8_t*)msg2,strlen(msg2)+1);
	
	
}