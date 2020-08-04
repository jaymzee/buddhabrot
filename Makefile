CC = gcc
CXX = g++
CFLAGS = -O2 -std=c99 -pedantic -Werror
CXXFLAGS = -O2 -std=c++17 -pedantic -Werror
LFLAGS = -lm
TARGETS = mandelbrot buddhabrot buddhabrotpp buddhabrotmt

all: $(TARGETS)

mandelbrot: obj/mandelbrot.o
	$(CC) -o $@ $^ $(LFLAGS)

obj/mandelbrot.o: src/mandelbrot.c
	$(CC) -o $@ $(CFLAGS) -c $<

buddhabrot: obj/buddhabrot.o obj/image.o obj/spinner.o obj/common.o
	$(CC) -o $@ $^ $(LFLAGS)

obj/buddhabrot.o: src/buddhabrot.c
	$(CC) -o $@ $(CFLAGS) -c $<

buddhabrotpp: obj/buddhabrotpp.o obj/image.o obj/spinner.o obj/common.o
	$(CXX) -o $@ $^ $(LFLAGS)

obj/buddhabrotpp.o: src/buddhabrotpp.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $<

buddhabrotmt: obj/buddhabrotmt.o obj/spinner.o obj/myrandom.o obj/image.o obj/common.o
	$(CXX) -o $@ $^ $(LFLAGS) -pthread

obj/buddhabrotmt.o: src/buddhabrotmt.cpp
	$(CXX) -o $@ $(CXXFLAGS) -pthread -c $<

obj/myrandom.o: src/myrandom.c
	$(CC) -o $@ $(CFLAGS) -c $<

obj/image.o: src/image.c
	$(CC) -o $@ $(CFLAGS) -c $<

obj/spinner.o: src/spinner.c
	$(CC) -o $@ $(CFLAGS) -c $<

obj/common.o: src/common.c
ifndef BUILD_VERSION
	$(error environment variable BUILD_VERSION not set)
endif
	$(CC) -o $@ $(CFLAGS) -DVERSION='"$(BUILD_VERSION)"' -c $<

clean:
	rm -f obj/*.o
	rm -f $(TARGETS)
