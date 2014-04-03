/;/Sarah Strickland
//http://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/getopt.html

#include "DNSClient.h"

void fheader(uint8_t* buff){

	uint16_t* header = (uint16_t*)buff; 
    int id;

//set id	
	id = rand() % (MAX_16_BIT + 1); 
	header[0] = htons(id); 

//set next 16 bits 
//all zeros except RD which equals 256
	header[1] =  htons((uint16_t) 0b100000000);
//QDCOUNT
	header[2] = htons((uint16_t)1);
//ANCOUNT
	header[3] = htons((uint16_t)0);
//NSCOUNT
	header[4] = htons((uint16_t)0);
//ARCOUNT
	header[5] = htons((uint16_t)0);

}
 
void fquery(uint8_t* buff, char*name){

//QNAME
	int offset=0;
	char* dot;
	char* dotname;
	uint8_t length;
	
	dotname = (char*) malloc(strlen(name)+2);
	memcpy(dotname, name, strlen(name)+1);
	dotname[strlen(name)+1] = '\0'; 

	dot = strtok(dotname, ".");

	while(dot != NULL){
		length = (uint8_t)strlen(dot);
		memcpy(buff, &length, 1);
		offset++;
		memcpy(buff+offset, dot, strlen(dot));
		offset= offset + strlen(dot);
		dot = strtok(NULL,".");
	}

//QTYPE
	buff[offset]= (uint8_t)0x0001;
	//memcpy(buff+offset, (uint8_t*)0x0001,sizeof(uint8_t*));
	offset = offset + sizeof(uint8_t);

//QCLASS
	buff[offset]=(uint8_t)0x0001;
	//memcpy(buff+offset, (uint16_t *)0x0001, sizeof(uint16_t*)); 	
}


int main(int argc, char **argv){

    int c,socketfd;
    extern int optind;
    extern char *optarg;
    int time = -1, retries = -1, port = -1;
    char* ip = NULL; 
	char*name = NULL;
	uint8_t* header = NULL;
	uint8_t* query = NULL;
    uint8_t* buff = NULL;
    struct sockaddr_in addr;   

//buffer to hold all the things
    buff = (uint8_t*) malloc (1024);
//header is the beginning of the buffer
	header = buff; 
//qurey is the after the header
	query = buff + 12; 

//parse arguments 
    while ((c = getopt(argc, argv, "t:r:p:")) != -1) {
        switch(c) {
        case 't':
	    time = atoi(optarg);
	    time = htons(time);
            break;
        case 'r':
	   retries = atoi(optarg);
           retries = htons(retries);
           break;
        case 'p':
	    port = atoi(optarg);
            port = htons(port);
	    break;
        case '?': //default error out
            printf("unkown argument, %s \n", optarg);
	    exit(1);
        }
		if((optind+2) > argc){
			printf("not enough arguments, must enter server and name \n");	//make sure there are enough args
			exit(1);
		}
	}

    ip = argv[optind];//ip address
    name = argv[optind+1]; //name 

	//set time retries and port if not specified 
    if(time == -1) time = 5;
    if(retries == -1) retries = 3;
    if(port == -1) port = 53;

//set header
	fheader(header);
//create query
//	 fquery(query, name);

	printf("buffer size: %li\n", sizeof(buff));

//create socket 

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if(inet_pton(AF_INET ,ip, &addr.sin_addr) <= 0){
		printf("inet_pton error");
		return 0;
	}

	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("couldn't create socket\n");
		return 0;
	}

//send to port
	if ((sendto(socketfd, buff, sizeof(buff), 0, (struct sockaddr *)&addr, sizeof(addr))) < 0){
		printf("sendto error\n");
		return 0;
	}
	

    return 0;
}



