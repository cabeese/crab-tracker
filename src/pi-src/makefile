CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic
CATCH_PATH=`pwd`/include

CFILES=main.cpp spi.cpp data_collection.cpp util.cpp uid.cpp direction.cpp
OFILES=${CFILES:.cpp=.o}
TESTFILES=test_*.cpp

all: ${OFILES}
	$(CXX) -o crab-tracker ${OFILES}

install:
	@mkdir -p include
	@wget https://raw.githubusercontent.com/CatchOrg/Catch2/master/single_include/catch.hpp -P include/

run:
	@./crab-tracker

testcode: ${OFILES}
	$(CXX) -I$(CATCH_PATH) -c test_main.cpp
	$(CXX) -I$(CATCH_PATH) -o test test_main.o spi.cpp util.cpp uid.cpp ${TESTFILES}

test-direction: direction.cpp direction.h test_direction.cpp
	$(CXX) -o test direction.cpp test_direction.cpp

test-dc: test_data_collection.cpp data_collection.cpp data_collection.h
	$(CXX) -std=c++11 -I$(CATCH_PATH) -c test_main.cpp
	$(CXX) -std=c++11 -I$(CATCH_PATH) -o test test_main.o data_collection.cpp spi.cpp util.cpp test_data_collection.cpp

test-uid: test_uid.cpp uid.cpp uid.h
	$(CXX) -std=c++11 -I$(CATCH_PATH) -c test_main.cpp
	$(CXX) -std=c++11 -I$(CATCH_PATH) -o test test_main.o uid.cpp test_uid.cpp

clean:
	@rm -f *.o *~ crab-tracker test
	@echo Working directory cleaned

#dependency list
${OFILES}: spi.h data_collection.h util.h config.h uid.h common.h direction.h
