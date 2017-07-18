args = -g -std=c++14 -I ../../include -Wall

myls: ls_common.o ls_display.o ls_param.o ls2.o
	g++ $(args) -o myls ls_common.o ls_display.o ls_param.o ls2.o

ls2.o : ls2.cpp
	g++ -c ls2.cpp $(args)
ls_common.o : ls_common.cpp
	g++ -c $(args) ls_common.cpp
ls_display.o : ls_display.cpp
	g++ -c $(args) ls_display.cpp
ls_param.o : ls_param.cpp
	g++ -c $(args) ls_param.cpp

.PHONY: clean
clean :
	-rm *.o