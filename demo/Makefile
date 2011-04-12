
INCLUDE=-I../

OBJECTS=
CC=g++
#CFLAGS=-Wall -ansi -pedantic $(INCLUDE)
CFLAGS=-Wall -ansi $(INCLUDE)
BINS=tuner engine

all: $(BINS)

%: %.cpp $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $<

#%.o: %.cpp %.h
#	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f $(BINS)
	-rm -f $(OBJECTS)
	-rm -fr doc

