CC=g++
FLAGS=-std=c++11 -Wall -Wdeprecated -pedantic -O2 
TEST=gtest.out
OBJECTS=bin/Genome.o bin/TestGenome.o
TESTOS=bin/TestGenome.o
HEADERS=src/Genome.h
BINARY=

$(TEST): $(OBJECTS)
	$(CC) test/gtest_main.cc -o bin/$@ -lgtest $(OBJECTS)

bin/Genome.o: src/Genome.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

bin/TestGenome.o: test/TestGenome.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS) $(TEST) $(TESTO)

all: $(TEST)
