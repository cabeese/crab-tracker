CXX=g++ -std=c++11
# This isn't working: CXXFLAGS=-std=c++11
CATCH_PATH=`pwd`/include #/catch.hpp

CFILES=main.cpp spi.cpp data_collection.cpp
OFILES=${CFILES:.c=.o}

all: ${OFILES}
	$(CXX) -o crab-tracker ${OFILES}

install:
	@mkdir -p include
	@wget https://raw.githubusercontent.com/CatchOrg/Catch2/master/single_include/catch.hpp -P include/

run:
	@./crab-tracker

testcode: ${OFILES}
	g++ -std=c++11 -Wall -I$(CATCH_PATH) -c test_main.cpp
	g++ -std=c++11 -Wall -I$(CATCH_PATH) -o test test_main.o test_data_collection.cpp data_collection.cpp

test: ${OFILES}
	@./test

clean:
	@rm -f *.o *~ crab-tracker test
	@echo Working directory cleaned

#dependency list
${OFILES}: spi.h data_collection.h # add other .h files?
