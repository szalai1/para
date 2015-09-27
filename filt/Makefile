CC=g++
FLAGS=
BINARY=main
OBJECTS=conv.o image.o
HEADERS=

$(BINARY): $(OBJECTS)
	$(CC) -g $^ -o $@

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

clean:
	rm -f *.o $(BINARY)

all: $(BINARY)
