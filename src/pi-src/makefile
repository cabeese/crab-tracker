CC=g++

CFILES=main.c spi.c
OFILES=${CFILES:.c=.o}

all: ${OFILES}
	$(CC) -o crab-tracker ${OFILES}

run:
	@./crab-tracker

clean:
	@rm -f *.o *~ crab-tracker
	@echo Working directory cleaned

#dependency list
${OFILES}: spi.h # add other .h files?
