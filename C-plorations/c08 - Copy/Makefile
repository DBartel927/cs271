CC = gcc
CFLAGS = -Wall -I.
LDFLAGS = 
OBJFILES = main.o parser.o symtable.o error.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	del /Q main.o parser.o symtable.o error.o $(TARGET) *~