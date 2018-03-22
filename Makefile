##
#   \file   Makefile
#   \author James Petersen
#
#   \brief  This is the Makefile that builds the TCP Server
#
#   Usage:
#       make
#       make clean
##

CC     = gcc
CFLAGS = -Wall
LIB    = -lpthread
OBJ    = server.o commands.o main.o
TARGET = server

server: $(OBJ)
	mkdir -p files
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIB)

server.o: src/server.c include/server.h
	$(CC) $(CFLAGS) -c src/server.c

commands.o: src/commands.c include/commands.h
	$(CC) $(CFLAGS) -c src/commands.c

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c

clean:
	rm -f $(OBJ) $(TARGET)
	rm -f files/*
