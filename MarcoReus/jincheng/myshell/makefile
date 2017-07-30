cc = gcc
OBJ = myshell
all :
	$(cc)  -c  ./*.c
	$(cc)  -o $(OBJ)  ./*.o  -I /usr/lib/x86_64-linux-gnu/libreadline.so  -lreadline -ltermcap  -g
	rm -rf  *.o
clean:
	rm -rf *.o
