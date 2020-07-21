CC = gcc
CXX = g++
CFLAGS = -O2 -std=c99 -pedantic -Werror
CXXFLAGS = -O2 -std=c++11 -pedantic -Werror
LFLAGS = -s -lm
OBJS = mandelbrot buddhabrot buddhabrotpp

all: $(OBJS)

mandelbrot: mandelbrot.o
	$(CC) -o $@ $^ $(LFLAGS)

mandelbrot.o: mandelbrot.c
	$(CC) -O2 -std=c89 -pedantic -Werror -c $<

buddhabrot: buddhabrot.o image.o common.o
	$(CC) -o $@ $^ $(LFLAGS)

buddhabrot.o: buddhabrot.c
	$(CC) $(CFLAGS) -c $<

buddhabrotpp: buddhabrotpp.o image.o common.o
	$(CXX) -o $@ $^ $(LFLAGS)

buddhabrotpp.o: buddhabrotpp.cpp
	$(CXX) $(CXXFLAGS) -c $<

image.o: image.c
	$(CC) $(CFLAGS) -c $<

common.o: common.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(OBJS)
