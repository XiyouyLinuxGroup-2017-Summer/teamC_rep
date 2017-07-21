sous = $(wildcard *.cpp)
objs = $(patsubst %.cpp,%.o,$(sous))
args = -std=c++14 -I ../include -Wall -lreadline

psh: $(objs)
	g++ $(args) -o psh $(objs)

$(objs) : $(sous)
	g++ $(args) -c *.cpp 

.PHONY: clean
clean :
	-rm $(objs)