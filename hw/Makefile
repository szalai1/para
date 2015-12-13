CC=g++
FLAGS=-std=c++11 -Wall -Wdeprecated -pedantic -O2 -g
TEST=gtest.out
OBJECTS=bin/Genome.o bin/TestGenome.o bin/TestPopulation.o bin/Queen.o
TESTOS=bin/TestGenome.o bin/TestPopulation.o
HEADERS=src/Genome.h src/Population.hpp src/Queen.h
BINARY=

$(TEST): $(OBJECTS) $(HEADERS)
	$(CC) test/gtest_main.cc -o bin/$@ -lgtest $(OBJECTS)

bin/Genome.o: src/Genome.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

bin/Queen.o: src/Queen.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

bin/TestGenome.o: test/TestGenome.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

bin/TestPopulation.o: test/TestPopulation.cpp src/Genome.h src/Genome.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS) $(TEST) $(TESTO)

all: $(TEST)

