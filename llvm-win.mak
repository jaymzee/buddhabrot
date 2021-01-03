# gnu makefile targetting LLVM clang compiler for windows
# buddhabrot omitted from OBJS because clang on windows imports the
# MSVC headers which do not support the double complex type

CC = clang
CXX = clang++
CFLAGS = -O2 -std=c99 -pedantic -Werror -D_CRT_SECURE_NO_WARNINGS
CXXFLAGS = -O2 -std=c++17 -pedantic -Werror -D_CRT_SECURE_NO_WARNINGS
LFLAGS =
TARGETS = $(addsuffix .exe, mandelbrot buddhabrot buddhabrotpp buddhabrotmt)

vpath %.h src
vpath %.c src
vpath %.cpp src
vpath %.o obj

all: $(TARGETS)

mandelbrot.exe: mandelbrot.o
	$(CC) -o $@ $^ $(LFLAGS)

obj/mandelbrot.o: mandelbrot.c
	$(CC) -o $@ $(CFLAGS) -c $<

buddhabrot.exe: buddhabrot.o random.o image.o spinner.o common.o
	$(CC) -o $@ $^ $(LFLAGS)

obj/buddhabrot.o: buddhabrot.c random.h image.h spinner.h common.h
	$(CC) -o $@ $(CFLAGS) -c $<

buddhabrotpp.exe: buddhabrotpp.o random.o image.o spinner.o common.o
	$(CXX) -o $@ $^ $(LFLAGS)

obj/buddhabrotpp.o: buddhabrotpp.cpp random.h image.h spinner.h common.h
	$(CXX) -o $@ $(CXXFLAGS) -c $<

buddhabrotmt.exe: buddhabrotmt.o random.o image.o spinner.o common.o
	$(CXX) -o $@ $^ $(LFLAGS)

obj/buddhabrotmt.o: buddhabrotmt.cpp random.h image.h spinner.h common.h
	$(CXX) -o $@ $(CXXFLAGS) -c $<

obj/random.o: random.c random.h
	$(CC) -o $@ $(CFLAGS) -c $<

obj/image.o: image.c image.h
	$(CC) -o $@ $(CFLAGS) -c $<

obj/spinner.o: spinner.c spinner.h
	$(CC) -o $@ $(CFLAGS) -c $<

obj/common.o: common.c common.h image.h
ifndef BUILD_VERSION
	$(error environment variable BUILD_VERSION not set)
endif
	$(CC) -o $@ $(CFLAGS) -DVERSION='"$(BUILD_VERSION)"' -c $<

clean:
	rm -f obj/*.o
	rm -f $(TARGETS)
