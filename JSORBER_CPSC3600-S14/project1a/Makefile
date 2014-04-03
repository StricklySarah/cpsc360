# Makefile for HW1 test programs

# Here is a rule for building .o files from .cpp files
# the -Wall option tells gcc to print all warning messages

# SRCS = logger.cpp phy.cpp l1.cpp l2.cpp l3.cpp l4.cpp 
SRCS := $(wildcard *.cpp)

.cpp.o:
	g++ -g -O0 -c -Wall $<

# if you just type "make", this is the default target
#  (since it is the first one defined). You can also 
# explicitly tell make to do this by typing "make all"

#all: hw1test l3test
all: phytest l1test l2test l3test l4test l4btest

# here is the rule for building hw1test

phytest: phytest.o phy.o logger.o
	g++ -o phytest $^

l1test: l1test.o l1.o phy.o logger.o
	g++ -o l1test $^

l2test: l2test.o l2.o l1.o phy.o logger.o
	g++ -o l2test $^
	
l3test: l3test.o l3.o l2.o l1.o phy.o logger.o
	g++ -o l3test $^

l4test: l4test.o l4.o l3.o l2.o l1.o phy.o logger.o
	g++ -o l4test $^
	
l4btest: l4btest.o l4b.o l3.o l2.o l1.o phy.o logger.o
	g++ -o l4btest $^
	
	
clean:
	rm *.o
	rm phytest
	rm l1test
	rm l2test
	rm l3test
	rm l4test
	rm l4btest

depend: $(SRCS)
	makedepend $(INCLUDES) $^
	

# DO NOT DELETE

l1.o: logger.h phy.h l1.h
l1test.o: logger.h l1.h phy.h
l2.o: logger.h phy.h l1.h l2.h
l2test.o: logger.h l2.h l1.h phy.h
l3.o: logger.h l2.h l1.h phy.h l3.h
l3test.o: logger.h l3.h l2.h l1.h phy.h
l4.o: logger.h l3.h l2.h l1.h phy.h l4.h
l4test.o: logger.h l4.h l3.h l2.h l1.h phy.h
logger.o: logger.h
phy.o: phy.h logger.h
phytest.o: logger.h phy.h
