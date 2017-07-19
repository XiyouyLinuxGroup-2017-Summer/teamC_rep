CC = g++
CXXFLAGS = -Wall -g -O -fPIC -lopenssl
INCLUDE = -I ../include
TARGET = libportal.so
LIBPATH = /usr/lib/

vpath %.h ../include

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

$(TARGET):$(OBJS)
	$(CC) -lcrypto -lssl -shared -fPIC -o $(TARGET) $(OBJS)
	sudo mv $(TARGET) $(LIBPATH)

$(OBJS):$(SRCS)
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $^

.PHONY: clean
clean:
	-rm -f *.o