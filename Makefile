SHELL:=/bin/bash
CC=clang++
CFLAGS=-O0 -g -c -Wall -pthread -Wno-unused-function -std=c++11
LDFLAGS=
SOURCES=conch.cpp
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