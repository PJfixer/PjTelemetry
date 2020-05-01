CC=gcc
CFLAGS= --std=c11 -I /usr/include
LIBS= -lpthread

all: udp.o serial.o bridge.o
	$(CC) $(CFLAGS) main.c $? -o MavlinkRem $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $? -o $@ $(LIBS)
	
	
clean:
	rm -f *.o main


