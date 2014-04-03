//Sarah Strickland


#include "DNSClient.h"

void fheader(uint8_t* buff){
printf("in fheader\n");
	int id;
	uint16_t* header = (uint16_t*)buff;
	
//set id	
	id = rand() % (MAX_16_BIT + 1); 
	header[0] = htons((uint16_t) id); 

//set next 16 bits 
//all zeros except RD which equals 256
	header[1] =  (uint16_t) htons(0b0000000100000000);
//QDCOUNT
	header[2] = (uint16_t) htons(1);
//ANCOUNT
	header[3] = (uint16_t) htons(0);
//NSCOUNT
	header[4] = (uint16_t) htons(0);
//ARCOUNT
	header[5] = (uint16_t) htons(0);

printf("created header\n");
return;
}
 
int fquery(uint8_t* buff, char*name){

//QNAME
	int offset=0, zero = 0;
	char* dot;
	char* dotname;
	uint8_t length;
	
	dotname = (char*) malloc(strlen(name)+2);
	memcpy(dotname, name, strlen(name)+1);
//insert a null charater at the end
	dotname[strlen(name)+1] = '\0'; 

	dot = strtok(dotname, ".");

	while(dot != NULL){
		length = (uint8_t)strlen(dot);
		memcpy(buff+offset, &length, 1);
		offset++;
		memcpy(buff+offset, dot, strlen(dot));
		offset= offset + strlen(dot);
		dot = strtok(NULL,".");
	}
	
	memcpy(buff+offset, &zero, sizeof(uint8_t));
	offset = offset + sizeof(uint8_t);

//QTYPE
	buff[offset]= (uint8_t) 0x00;
	offset = offset + sizeof(uint8_t);
	buff[offset]= (uint8_t) 1;
	offset = offset + sizeof(uint8_t);

//QCLASS
	buff[offset]=(uint8_t) 0x00;
	offset = offset + sizeof(uint8_t);
	buff[offset]=(uint8_t)1;	
	offset = offset + sizeof(uint8_t);
	
	return offset;
}

int* parseAnswerHeader(uint8_t* answer, uint8_t* anspt){
	
	uint16_t* header = (uint16_t*) answer;
	uint16_t QRbit, OpCodebit, AAbit, TCbit, RDbit, RAbit, Zbit, RCodebit,  ANcount;
	int returnThis[2];

	
	QRbit = ntohs(header[1]) & QR; //QR
	QRbit = QRbit >> 15;

	
	OpCodebit = ntohs(header[1]) & OPCODE;
	OpCodebit = OpCodebit >> 11;

	
	AAbit = ntohs(header[1]) & AA;
	AAbit = AAbit >> 10;
	returnThis[0] = AAbit;

	
	TCbit = ntohs(header[1]) & TC;
	TCbit = TCbit >> 9;

	
	RDbit = ntohs(header[1]) & RD;
	RDbit = RDbit >> 8;

	
	RAbit = ntohs(header[1]) & RA;
	RAbit = RAbit >> 7;

	
	Zbit = ntohs(header[1]) & Z;
	Zbit = Zbit >> 4;

	
	RCodebit = ntohs(header[1]) & RCODE;

	
	ANcount = ntohs(header[3]);
	returnThis[1] = ANcount;
	


//Make sure all the bits are right	
	if(TCbit == 1){//see if answer was truncated
		printf("ERROR	Answer Truncated\n");
		return NULL;
	}else if(RAbit == 0){//make sure recursion is avaiable 
		printf("ERROR	Recursion Unavalible\n");
		return NULL;
	}else if(RCodebit == 1){//make sure format is right
		printf("ERROR	Format error\n");
		return NULL;
	}else if(RCodebit == 2){//make sure server didnt fail
		printf("ERROR	Server failure\n");
		return NULL;
	}else if(RCodebit == 3){//name doesnt exist
		printf("NOTFOUND\n");
		return NULL;
	}
	
	
	return returnThis; 

}

void parseName(uint8_t* buff, uint8_t* startRead, uint8_t* readInto, int startWrite){
	
	uint16_t startReadOffset = 0;
	uint16_t bit1, bit2, offset;
	int i;	
	
	uint8_t byte1;
	memcpy(&byte1, startRead, sizeof(uint8_t));///uint8 or uint16	
	
	//check to see if it's a pointer
	memcpy(&bit1, startRead, sizeof(uint16_t));
	bit1 = ntohs(bit1);
	bit2 = bit1;
	
	//get first bit
	bit1 = bit1 >> 15;
	//get second bit
	bit2 = bit2 << 1;
	bit2 = bit2 >> 15;
	
	if(bit1 == 1 && bit2 == 1){ // pointer found
		memcpy(&offset, startRead, sizeof(uint16_t));///uint8 or uint16
		offset = ntohs(offset);
		offset = offset & OFFSET_MASK;

		parseName(buff, buff+offset, readInto, startWrite);
		
	}else if(byte1  == 0){
		memcpy(readInto+startWrite-1, "\0", sizeof("\0"));
		return; 
	}else{ //pointer not found READ ME!
			
		for(i = -1; i < byte1; i++){
			memcpy(readInto+startWrite, startRead+startReadOffset, sizeof(uint8_t));///uint6 or uint16
			startWrite = startWrite + sizeof(uint8_t);
			startReadOffset = startReadOffset + sizeof(uint8_t);
		}
		memcpy(readInto+startWrite, ".", sizeof("."));
		startWrite = startWrite + 1;
		parseName(buff, startRead+startReadOffset, readInto, startWrite);

	}
	
	
}

int* parseAnswerData(uint8_t* answerStart){	
	

	
	uint16_t type = 0;
	int ttl = 0;
	uint16_t RDLength = 0;
	int* returnMe= (int*)malloc(2);		
	
//type
	memcpy(&type, answerStart+sizeof(uint16_t), sizeof(uint16_t));
	type = ntohs(type);
	returnMe[0] = type;
	
//TTL
	memcpy(&ttl, answerStart+(sizeof(uint16_t)*4), sizeof(int));
	ttl = ntohs(ttl);
	returnMe[1] = ttl;
	
//RDLength
	memcpy(&RDLength, answerStart+(sizeof(uint16_t)*5), sizeof(uint16_t));
	RDLength = ntohs(RDLength);

	
	return returnMe;
}


int main(int argc, char **argv){

    int c,socketfd, qlength, auth;
    extern int optind;
    extern char *optarg;
    int time = -1, retries = -1, port = -1;
    char* ip = NULL; 
	uint8_t* newIP = NULL;
	char*name = NULL;
	uint8_t* header = NULL;
	uint8_t* query = NULL;
	uint8_t* answerpoint = NULL;
    uint8_t* buff = NULL;
	uint8_t* answer= NULL;
	uint8_t* cname = NULL;
	uint8_t* qname = NULL;
    struct sockaddr_in sendaddr;  
	int* data = (int*)malloc(2); 
	int* data2 = (int*)malloc(2);
	char* autho= NULL;

//buffer to hold all the things
    buff = (uint8_t*) malloc (1024);
	
	answer = (uint8_t*) malloc(1024);
//header is the beginning of the buffer
	header = buff; 
//qurey is the after the header
	query = buff + 12;
//buffer to hold ip we find
	newIP = (uint8_t*)malloc(4);	

//parse arguments 
    while ((c = getopt(argc, argv, "t:r:p:")) != -1) {
        switch(c) {
        case 't':
	    time = atoi(optarg);
            break;
        case 'r':
	   retries = atoi(optarg);
           break;
        case 'p':
	    port = atoi(optarg);
            break;
        case '?': //default error out
            printf("ERROR	unkown argument, %s \n", optarg);
	    exit(1);
        }
		if((optind+2) > argc){
			printf("ERROR	not enough arguments, must enter server and name \n");	//make sure there are enough args
			exit(1);
		}
	}

    ip = argv[optind];//ip address
	ip++;
    name = argv[optind+1]; //name 

	//set time retries and port if not specified 
    if(time == -1) time = 5;
    if(retries == -1) retries = 3;
    if(port == -1) port = 53;

	printf("time: %d\n", time);
	printf("retries: %d\n", retries);
	printf("port: %x\n", port);


//set header
 	  fheader(header);
	  printf("completed fheader\n");
//create query
	  qlength = fquery(query, name); 
	 
	  printf("completed fquery\n");

	  qlength = qlength + 12;
	 
	  answerpoint = answer + qlength;
//create socket 

	bzero(&sendaddr, sizeof(sendaddr));
printf("bzero\n");
	sendaddr.sin_family = AF_INET;
printf("sendarrd.sin_family = AF_INET;\n");	
	sendaddr.sin_port = htons(port);
printf("sendaddr.sin_port = htons(port); = %x\n", sendaddr.sin_port);

	if(inet_pton(AF_INET ,ip, &sendaddr.sin_addr) <= 0){
		printf("ERROR	inet_pton error\n");
		return 0;
	}

	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("ERROR	couldn't create socket\n");
		return 0;
	}

//send to port
	if ((sendto(socketfd, buff, qlength, 0, (struct sockaddr *)&sendaddr, sizeof(sendaddr))) < 0){
		printf("ERROR	sendto error\n");
		return 0;
	}
printf("sendto() set up\n");	
	
	//Now read the server's response.
	if (recvfrom(socketfd, answer, 1024, 0, NULL,NULL) < 0){
		printf("ERROR	Revfrom error\n");
		return 0;
	}
	
printf("revfrom()\n");
	qname = (uint8_t*) malloc(128);
	cname = (uint8_t*)malloc(128);

	data2 = parseAnswerHeader(answer, answerpoint);
	
	if(data2 == NULL){
		return 0;
	}

	auth = data2[0];


	
	if(auth == 1){
		autho = "auth";
	}else if(auth == 0){
		autho = "nonauth";
	}else{
		return 0;
	}

		parseName(buff, answerpoint, qname, 0);//get past the name in the answer
	
		data = parseAnswerData(answerpoint);
	
		int type = data[0];
		int ttl = data[1];
		
		answerpoint = answerpoint + (sizeof(uint8_t)*10);


		if(type == 5){//CNAME
			answerpoint = answerpoint + (sizeof(uint8_t)*2);
			parseName(buff, answerpoint, cname, 0); //get cname
			printf("CNAME\t%s\t%d\t%s\n",cname,ttl,autho);

		}else if(type == 1){//IP
			memcpy(newIP, answerpoint, 4);
			printf("IP\t%s\t%d\t%s\n", newIP, ttl, autho);

		}
		

    return 0;
}
