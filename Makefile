SHELL:=/bin/bash
CC=g++
CFLAGS=-O0 -g -c -Wall -Wno-unused-function -std=c++11
LDFLAGS=
SOURCES=main3.cpp Documentation.cpp Logger.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=conch

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o $(EXECUTABLE)
	$(RM) $(EXECUTABLE)
