CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic
CATCH_PATH=`pwd`/include

CFILES=main.cpp spi.cpp data_collection.cpp util.cpp
OFILES=${CFILES:.cpp=.o}

all: ${OFILES}
	$(CXX) -o crab-tracker ${OFILES}

config-test: ${OFILES}
	$(CXX) -o config-test read_config.cpp

install:
	@mkdir -p include
	@wget https://raw.githubusercontent.com/CatchOrg/Catch2/master/single_include/catch.hpp -P include/

run:
	@./crab-tracker

testcode: ${OFILES}
	g++ -std=c++11 -Wall -I$(CATCH_PATH) -c test_main.cpp
	g++ -std=c++11 -Wall -I$(CATCH_PATH) -o test test_main.o test_data_collection.cpp data_collection.cpp spi.cpp util.cpp

check:
	@./test

clean:
	@rm -f *.o *~ crab-tracker test
	@echo Working directory cleaned

#dependency list
${OFILES}: spi.h data_collection.h util.h config.h
