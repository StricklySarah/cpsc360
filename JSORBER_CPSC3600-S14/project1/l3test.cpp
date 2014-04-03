#include <stdint.h>
#include <string.h>
#include "logger.h"
#include "l3.h"


/****
Sample program that directly tests the third layer
*****/

class TestClient : public L3_upper
{
	int m_id;
	
public:
	TestClient(int id);
	void pop(uint8_t *message, int length);
};

TestClient::TestClient(int id)
{
	m_id = id;
}

void TestClient::pop(uint8_t *message, int length)
{
	char msg[1000];
	memset(msg, 0, 1000);
	memcpy(msg,message,length);
	
	
	output(LOG_CLIENT, "client #%i received message: %s (%i bytes)",m_id, msg,length);
}




int main(int argc, char *argv[])
{
	
	TestClient t1(1),t2(2);
	PHY p1,p2;
	L1 l1a,l1b;
	L2 l2a,l2b;
	L3 l3a,l3b;
	
	//const char *msg1 = "To: Jacob Sorber. From: Someone else. Subject: This is a very pointless message. But it is long enough for a decent test.\n Sincerely, Myself.";
	const char *msg1 = "Hi How are you.";
	const char *msg2 = "Thanks, but did you check for my validity?!?"; 
	
	
	//initialize PHY first
	p1.init(&l1a);
	p2.init(&l1b);
	
	//initialize L1
	l1a.init(&l2a,&p1);
	l1b.init(&l2b,&p2);
	
	//initialize l2
	l2a.init(&l3a,&l1a);
	l2b.init(&l3b,&l1b);
	
	//initialize l3
	l3a.init(&t1,&l2a);
	l3b.init(&t2,&l2b);
	
	
	
	
	l3a.push((uint8_t*)msg1,strlen(msg1)+1);
	l3b.push((uint8_t*)msg2,strlen(msg2)+1);
	
	
}