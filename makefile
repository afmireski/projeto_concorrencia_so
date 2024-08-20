CC = gcc
CFLAGS = -Wall
OBJ = aluno.o professor.o
TARGET = main

all: $(TARGET)

$(TARGET): main.c $(OBJ)
	$(CC) $(CFLAGS) main.c $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
