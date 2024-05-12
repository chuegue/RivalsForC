CC=gcc
CCFLAGS= -g -ggdb -Wall -Wextra -pedantic -Werror -Wshadow -Wcast-align -Wswitch-enum -Wunreachable-code -Wfloat-equal -Wno-unused-function -std=c99 $(shell pkg-config --cflags sdl2 SDL2_ttf)
LDFLAGS=$(shell pkg-config --libs sdl2 SDL2_ttf)
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=game

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
