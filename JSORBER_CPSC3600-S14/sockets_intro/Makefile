
CC=gcc

CFLAGS=-c -Wall
COMMON_OBJS = common.o wrappers.o


all: $(COMMON_OBJS) tcpc tcps udpc udps tcpc_v2
	
#Common wrappers and definitions that all of the other examples use.
common.o: common.h common.c
	$(CC) $(CFLAGS) common.c

wrappers.o: common.h wrappers.c
	$(CC) $(CFLAGS) wrappers.c

#targets for 4 different clients and servers
tcpc: tcpclient.c $(COMMON_OBJS)
	$(CC) -o tcpc tcpclient.c $(COMMON_OBJS)

tcpc_v2: tcpclientv2.c $(COMMON_OBJS)
	$(CC) -o tcpc_v2 tcpclientv2.c $(COMMON_OBJS)

tcps: tcpserver.c $(COMMON_OBJS)
	$(CC) -o tcps tcpserver.c $(COMMON_OBJS)

udpc: udpclient.c $(COMMON_OBJS)
	$(CC) -o udpc udpclient.c $(COMMON_OBJS)

udps: udpserver.c $(COMMON_OBJS)
	$(CC) -o udps udpserver.c $(COMMON_OBJS)	

clean:
	rm -rf *.o tcpc tcps udpc udps tcpc_v2