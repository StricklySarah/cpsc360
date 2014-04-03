#include <stdint.h>
#include <string.h>
#include "logger.h"
#include "l4b.h"


/****
Sample program that directly tests the layer 4b
*****/

#define CMD_LEN 4096

class TestClient : public L4b_upper
{
	int m_id;
	
public:
	TestClient(int id);
	void pop(uint8_t **commands, int *command_lengths, int command_list_length);
};

TestClient::TestClient(int id)
{
	m_id = id;
}

void TestClient::pop(uint8_t **commands, int *command_lengths, int command_list_length)
{
	
	char command_buffer[CMD_LEN];

	for (int i=0; i < command_list_length; i++)
	{
		memset(command_buffer, 0, CMD_LEN);
		printf("Test: Command (%d):  Length: %d\n", i, command_lengths[i]);
		memcpy(command_buffer, commands[i], command_lengths[i]);
		
		printf("test pop\n");

		output(LOG_CLIENT, "Client #%i -- Command %d -- %s\n", m_id, i, command_buffer);
	}
}




int main(int argc, char *argv[])
{
	
	TestClient t1(1),t2(2);
	PHY p1,p2;
	L1 l1a,l1b;
	L2 l2a,l2b;
	L3 l3a,l3b;
	L4b l4a,l4b;
	
	const char* commands[] = { "PRINT Hello World", "SAVE SOMETHING", "READ SOMETHING ELSE" };
	const int lengths[] = { 17, 14,19 };
	
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
	
	
	l4a.push((uint8_t**)commands,(int*)lengths,3);
	l4b.push((uint8_t**)commands,(int*)lengths,3);

	float tbytes = p1.total_bytes_sent() + p2.total_bytes_sent();
	float dbytes = 2*(17+14+19);
	float overhead = 100 * (1-(dbytes/tbytes));
	
	printf("Sent a total of %0.0f bytes, to deliver %0.0f bytes (%0.2f percent overhead). \n",tbytes,dbytes,overhead);
	
}
