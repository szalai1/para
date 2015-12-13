CC=g++
FLAGS=-std=c++11 -Wall -Wdeprecated -pedantic -O2 -g
TEST=gtest.out
OBJECTS=bin/Individual.o bin/TestPopulation.o bin/Population.o
TESTOS=bin/TestIndividual.o bin/TestPopulation.o
HEADERS=src/Individual.h src/Population.hpp
BINARY=

$(TEST): $(OBJECTS) $(HEADERS)
	$(CC) test/gtest_main.cc -o bin/$@ -lgtest $(OBJECTS)

bin/Individual.o: src/Individual.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

bin/Population.o: src/Population.cpp src/Population.h
	$(CC) $(FLAGS) $< -c -o $@

bin/TestPopulation.o: test/TestPopulation.cpp src/Individual.h src/Individual.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS) $(TEST) $(TESTO)

all: $(TEST)

