CC=gcc
CFLAGS= -g -Wall
LDFLAGS = -lpthread -lrt

H_SOURCE =  ctrl_acess.h

EXCS= serv ctlp

EXSD= serv intges

EXMQ= serv ctlp

EXALL= $(EXSD) $(EXMQ) $(EXCS)

all: $(EXALL)

sd: $(EXSD) 

clean:
	rm -f $(OBJS) $(EXALL)









