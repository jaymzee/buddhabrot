CC = gcc
CXX = g++
CFLAGS = -O2 -std=c99 -pedantic -Werror
CXXFLAGS = -O2 -std=c++17 -pedantic -Werror
LFLAGS = -lm
OBJS = mandelbrot.o buddhabrot.o buddhabrotpp.o buddhabrotmt.o \
       common.o image.o myrandom.o spinner.o
TARGETS = mandelbrot buddhabrot buddhabrotpp buddhabrotmt

all: $(TARGETS)

mandelbrot: mandelbrot.o
	$(CC) -o $@ $^ $(LFLAGS)

mandelbrot.o: mandelbrot.c
	$(CC) $(CFLAGS) -c $<

buddhabrot: buddhabrot.o image.o spinner.o common.o
	$(CC) -o $@ $^ $(LFLAGS)

buddhabrot.o: buddhabrot.c
	$(CC) $(CFLAGS) -c $<

buddhabrotpp: buddhabrotpp.o image.o spinner.o common.o
	$(CXX) -o $@ $^ $(LFLAGS)

buddhabrotpp.o: buddhabrotpp.cpp
	$(CXX) $(CXXFLAGS) -c $<

buddhabrotmt: buddhabrotmt.o spinner.o myrandom.o image.o common.o
	$(CXX) -o $@ $^ $(LFLAGS) -pthread

buddhabrotmt.o: buddhabrotmt.cpp
	$(CXX) $(CXXFLAGS) -pthread -c $<

myrandom.o: myrandom.c
	$(CC) $(CFLAGS) -c $<

image.o: image.c
	$(CC) $(CFLAGS) -c $<

spinner.o: spinner.c
	$(CC) $(CFLAGS) -c $<

common.o: common.c
ifndef BUILD_VERSION
	$(error environment variable BUILD_VERSION not set)
endif
	$(CC) $(CFLAGS) -DVERSION='"$(BUILD_VERSION)"' -c $<

clean:
	rm -f $(OBJS)
	rm -f $(TARGETS)
