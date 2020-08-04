CC = gcc
CXX = g++
CFLAGS = -O2 -std=c99 -pedantic -Werror
CXXFLAGS = -O2 -std=c++17 -pedantic -Werror
LFLAGS = -lm
TARGETS = mandelbrot buddhabrot buddhabrotpp buddhabrotmt

all: $(TARGETS)

mandelbrot: $(addprefix obj/, mandelbrot.o)
	$(CC) -o $@ $^ $(LFLAGS)

obj/mandelbrot.o: $(addprefix src/, mandelbrot.c)
	$(CC) -o $@ $(CFLAGS) -c $<

buddhabrot: $(addprefix obj/, buddhabrot.o image.o spinner.o common.o)
	$(CC) -o $@ $^ $(LFLAGS)

obj/buddhabrot.o: $(addprefix src/, buddhabrot.c image.h spinner.h common.h)
	$(CC) -o $@ $(CFLAGS) -c $<

buddhabrotpp: $(addprefix obj/, buddhabrotpp.o image.o spinner.o common.o)
	$(CXX) -o $@ $^ $(LFLAGS)

obj/buddhabrotpp.o: $(addprefix src/, buddhabrotpp.cpp image.h spinner.h common.h)
	$(CXX) -o $@ $(CXXFLAGS) -c $<

buddhabrotmt: $(addprefix obj/, buddhabrotmt.o myrandom.o image.o spinner.o common.o)
	$(CXX) -o $@ $^ $(LFLAGS) -pthread

obj/buddhabrotmt.o: $(addprefix src/, buddhabrotmt.cpp myrandom.h image.h spinner.h common.h)
	$(CXX) -o $@ $(CXXFLAGS) -pthread -c $<

obj/myrandom.o: $(addprefix src/, myrandom.c myrandom.h)
	$(CC) -o $@ $(CFLAGS) -c $<

obj/image.o: $(addprefix src/, image.c image.h)
	$(CC) -o $@ $(CFLAGS) -c $<

obj/spinner.o: $(addprefix src/, spinner.c spinner.h)
	$(CC) -o $@ $(CFLAGS) -c $<

obj/common.o: $(addprefix src/, common.c common.h image.h)
ifndef BUILD_VERSION
	$(error environment variable BUILD_VERSION not set)
endif
	$(CC) -o $@ $(CFLAGS) -DVERSION='"$(BUILD_VERSION)"' -c $<

clean:
	rm -f obj/*.o
	rm -f $(TARGETS)
