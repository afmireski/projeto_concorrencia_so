CC = clang -Wall

FLAGS = -lpthread.

LIBS = aluno.o professor.o

EXE = main

PROGS=$(EXE)

all: $(PROGS)

$(EXE):
	$(CC) $(EXE).c $(LIBS) -o $(EXE)


CFLAGS=-I.
DEPS = aluno.h professor.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: aluno.o professor.o

clean: 
	rm *.o $(PROGS)