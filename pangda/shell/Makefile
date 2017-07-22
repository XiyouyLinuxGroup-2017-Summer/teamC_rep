SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
CXXFLAG = -std=c++14 -I ../include -O2 -lreadline

psh: $(OBJECTS)
	g++ $(CXXFLAG) -o psh $(OBJECTS)

$(OBJECTS): $(SOURCES)
	g++ $(CXXFLAG) -c $(SOURCES)

.PHONY: clean
clean :
	-rm $(OBJECTS)