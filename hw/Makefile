CC=mpic++
FLAGS=-std=c++11 -Wall -Wdeprecated -pedantic -O2 -g
TEST=gtest.out
OBJECTS=bin/Individual.o bin/Population.o 
TESTOS=bin/TestIndividual.o
HEADERS=src/Individual.h src/Population.h
BINARY=


main: $(OBJECTS) $(HEADERS) bin/main.o	
	$(CC) -g bin/main.o -o bin/$@  $(OBJECTS)

bin/main.o: src/main.cpp $(OBJECTS) $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@ 

bin/Individual.o: src/Individual.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

bin/Population.o: src/Population.cpp src/Population.h
	$(CC) $(FLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS) $(TEST) $(TESTO)

all: main

