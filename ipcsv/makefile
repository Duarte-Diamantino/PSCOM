CC=gcc
CFLAGS= -Wall -g

EXMSG= msg msgs msgc
EXSHM= shm shmp shmc
EXSEM= sem sem1 sem2

EXALL= $(EXMSG) $(EXSHM) $(EXSEM)

all: $(EXALL)

msgx: $(EXMSG) 

shmx: $(EXSHM)

semx: $(EXSEM) 

clean:
	rm -f $(EXALL)

