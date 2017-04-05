PROGRAM_NAME=banksim
CFLAGS=-c -Wall -std=c++11
OBJECTS=PA05.o Event.o PriorityQueue.o Queue.o

all: banksim numGenerator

# Main program for assignment
banksim: $(OBJECTS)
	g++ $(OBJECTS) -o $(PROGRAM_NAME)

# Random event generator
numGenerator: numGenerator.o
	g++ numGenerator.o -o numGenerator

%.o: %.cpp %.h
	g++ $(CFLAGS) $<

clean:
	rm *.o
