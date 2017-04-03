PROGRAM_NAME=banksim
CFLAGS=-c -Wall -std=c++11

all: banksim numGenerator

# Main program for assignment
banksim: PA05.o Event.o
	g++ PA05.o Event.o -o $(PROGRAM_NAME)

# Random event generator
numGenerator: numGenerator.o
	g++ numGenerator.o -o numGenerator

%.o: %.cpp %.h
	g++ $(CFLAGS) $<

clean:
	rm *.o
