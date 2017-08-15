all : server client

.PHONY : clean
server : server.o
	gcc server.c -o server -lpthread

client : client.o
	gcc client.c -o client -lpthread

clean:
	rm *.o

