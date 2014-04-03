/* 
This is an example TCP client, that transmits a message to a chat server (tcpserver.c)
*/

/**
 common.h includes commonly needed header files and helper functions
		like err_n_die(), which is a variant of printf.
		It also defines some values like MAXLINE that I use a lot.
**/
#include "common.h" 



int main(int argc, char **argv)
{
	int					sockfd, n;
	int 				sendbytes;
	struct sockaddr_in	servaddr;
	char				sendline[MAXLINE + 1];
	char				recvline[MAXLINE + 1];
	
	

	if (argc != 3) 
		err_n_die("usage: %s <server address> <name>",argv[0]);
	

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_n_die("Error while creating the socket!");
		
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(SERVER_PORT);	/* chat server */
	
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_n_die("inet_pton error for %s ", argv[1]);
		
		

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_n_die("connect failed!");


	//We're connected. Prepare the message.
	sprintf(sendline, "%s says Hi!\n", argv[2]);
	sendbytes = strlen(sendline); 
	
	//Send the message -- making sure you send it all
	//This code is a bit fragile, since it bails if only some of the bytes are sent.
	//normally, you would want to retry, unless the return value was -1.
	if (write(sockfd, sendline, sendbytes) != sendbytes)
		err_n_die("write error");
	
	//Now read the server's response.
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) 
	{
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_n_die("fputs error");
		
	}
	if (n < 0)
		err_n_die("read error");
	

	exit(0); //end successfully!
}
