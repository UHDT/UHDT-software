all: test

test: driver.o protocol.o dataqueue.o
	gcc -o test driver.o protocol.o dataqueue.o

driver.o: driver.c 
	gcc -c driver.c

protocol.o: protocol.c
	gcc -c protocol.c

dataqueue.o: dataqueue.c
	gcc -c dataqueue.c

clean:
	rm -f *.o test
