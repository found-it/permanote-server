##
#   \file   Makefile
#   \author James Petersen
#
#   \brief  This is the Makefile that builds the TCP Server
##

CC     = gcc
CFLAGS = -Wall

OBJ    = server.o main.o

TARGET = server

server: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

server.o: src/server.c include/server.h
	$(CC) $(CFLAGS) -c src/server.c

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c

clean:
	rm -f $(OBJ) $(TARGET)
