CC=g++

CFILES=main.cpp spi.cpp data_collection.cpp
OFILES=${CFILES:.c=.o}

all: ${OFILES}
	$(CC) -o crab-tracker ${OFILES}

install:
	@echo Downloading \"Catch2\" for testing
	@wget https://raw.githubusercontent.com/CatchOrg/Catch2/master/single_include/catch.hpp

run:
	@./crab-tracker

clean:
	@rm -f *.o *~ crab-tracker
	@echo Working directory cleaned

#dependency list
${OFILES}: spi.h data_collection.h # add other .h files?
