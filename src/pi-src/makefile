CXX=g++ -std=c++11 -Wall
# This isn't working: CXXFLAGS=-std=c++11
CATCH_PATH=`pwd`/include #/catch.hpp

CFILES=main.cpp spi.cpp data_collection.cpp util.cpp uid.cpp
OFILES=${CFILES:.c=.o}
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

test-dc: test_data_collection.cpp data_collection.cpp data_collection.h
	$(CXX) -I$(CATCH_PATH) -c test_main.cpp
	$(CXX) -I$(CATCH_PATH) -o test test_main.o spi.cpp util.cpp test_data_collection.cpp

test-uid: test_uid.cpp uid.cpp uid.h
	$(CXX) -I$(CATCH_PATH) -c test_main.cpp
	$(CXX) -I$(CATCH_PATH) -o test test_main.o uid.cpp test_uid.cpp

clean:
	@rm -f *.o *~ crab-tracker test
	@echo Working directory cleaned

#dependency list
${OFILES}: spi.h data_collection.h util.h config.h uid.h
