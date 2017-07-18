sous = $(wildcard *.cpp)
objs = $(patsubst %.cpp,%.o,$(sous))
args = -std=c++14 -I ../include -Wall

myls: $(objs)
	@echo $(objs)
	g++ $(args) -o myls $(objs)

$(objs) : $(sous)
	g++ $(args) -c *.cpp 

.PHONY: clean
clean :
	-rm $(objs)