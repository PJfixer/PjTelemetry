CC=gcc
CFLAGS= --std=c11 -I /usr/include
LIBS= 

all: serial.o
	$(CC) $(CFLAGS) main.c $? -o main $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $? -o $@ $(LIBS)
	
	
clean:
	rm -f *.o main


