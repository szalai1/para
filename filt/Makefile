CC=g++
FLAGS=-g
BINARY=main
OBJECTS=conv.o image.o
HEADERS=

$(BINARY): $(OBJECTS)
	$(CC) -g $^ -o $@

%.o: %.c $(HEADERS)
	$(CC) $< -c $(FLAGS) -o $@

clean:
	rm -f *.o $(BINARY)

all: $(BINARY)
