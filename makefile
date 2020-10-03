iCC = gcc
WARNING_FLAGS = -g -Wall -Wextra
EXE = prodcom

all: main.o module.o
	$(CC) -o $(EXE) main.o module.o

main.o: main.c module.h
	$(CC) $(WARNING_FLAGS) -c main.c

module.o: module.c module.h
	$(CC) $(WARNING_FLAGS) -c module.c

clean:
	rm $(EXE) *.o
