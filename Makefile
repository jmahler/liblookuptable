
INCLUDE=

OBJECTS=
CC=g++
#CFLAGS=-Wall -ansi -pedantic $(INCLUDE)
CFLAGS=-Wall -ansi $(INCLUDE)

all: test

doc: *.cpp *.h
	doxygen  # using Doxyfile 

test: test.cpp $(OBJECTS) lookuptable.h
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f test
	-rm -f $(OBJECTS)
	-rm -fr doc

