#include <stdint.h>
#include <string.h>
#include "logger.h"
#include "l4.h"


/****
Sample program that directly tests the third layer
*****/

class TestClient : public L4_upper
{
	int m_id;
	
public:
	TestClient(int id);
	void pop(uint8_t *name, int namelength, uint8_t *value, int valuelength);
};

TestClient::TestClient(int id)
{
	m_id = id;
}

void TestClient::pop(uint8_t *name, int namelength, uint8_t *value, int valuelength)
{
	
	output(LOG_CLIENT, "client #%i received named value (name=%.*s, value=%.*s) ",m_id, namelength, name, valuelength, value);
}




int main(int argc, char *argv[])
{
	
	TestClient t1(1),t2(2);
	PHY p1,p2;
	L1 l1a,l1b;
	L2 l2a,l2b;
	L3 l3a,l3b;
	L4 l4a,l4b;
	
	//const char *msg1 = "To: Jacob Sorber. From: Someone else. Subject: This is a very pointless message. But it is long enough for a decent test.\n Sincerely, Myself.";
	const char *name1 = "Genus";
	const char *value1 = "Herpestes";
	const char *name2 = "Species";
	const char *value2 = "javanicus"; 
	
	
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
	l3a.init(&l4a,&l2a);
	l3b.init(&l4b,&l2b);
	
	//initialize l4
	l4a.init(&t1,&l3a);
	l4b.init(&t2,&l3b);
	
	
	l4a.push((uint8_t*)name1,strlen(name1), (uint8_t*)value1, strlen(value1));
	l4b.push((uint8_t*)name2,strlen(name2), (uint8_t*)value2, strlen(value2));

	float tbytes = p1.total_bytes_sent() + p2.total_bytes_sent();
	float dbytes = strlen(name1) + strlen(name2);
	float overhead = 100 * (1-(dbytes/tbytes));
	
	printf("Sent a total of %0.0f bytes, to deliver %0.0f bytes (%0.2f percent overhead). \n",tbytes,dbytes,overhead);
	
}