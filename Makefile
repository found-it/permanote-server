##
#
#
#
##

CC = gcc
CFLAGS = -Wall

OBJ = server.o main.o

TARGET = server

server: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

server.o: server.c server.h
	$(CC) $(CFLAGS) -c server.c

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(OBJ) $(TARGET)
