SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
CXXFLAG = -std=c++14 -I ../include -Wall -O2

pls: $(OBJECTS)
	g++ $(CXXFLAG) -o pls $(OBJECTS)

$(OBJECTS): $(SOURCES)
	g++ $(CXXFLAG) -c *.cpp

.PHONY: clean
clean :
	-rm $(OBJECTS)