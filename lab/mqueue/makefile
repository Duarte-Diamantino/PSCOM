CC=gcc
CFLAGS= -Wall -g
LDFLAGS= -lrt

EXMQ= mqs mqc

EXALL= $(EXMQ)

all: $(EXALL)

mqs: mqs.o
	$(CC) -o mqs mqs.o $(LDFLAGS)

mqc: mqc.o
	$(CC) -o mqc mqc.o $(LDFLAGS)

clean:
	rm -f $(EXALL) *.o

