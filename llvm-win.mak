# gnu makefile targetting LLVM clang compiler for windows
# buddhabrot omitted from OBJS because clang on windows imports the
# MSVC headers which do not support the double complex type

CC = clang
CXX = clang++
CFLAGS = -O2 -D_CRT_SECURE_NO_WARNINGS -std=c99 -pedantic -Werror
CXXFLAGS = -O2 -D_CRT_SECURE_NO_WARNINGS -std=c++17 -pedantic -Werror
LFLAGS = 
OBJS = mandelbrot.exe buddhabrot.exe buddhabrotpp.exe buddhabrotmt.exe

all: $(OBJS)

mandelbrot.exe: mandelbrot.o
	$(CC) -o $@ $^ $(LFLAGS)

mandelbrot.o: mandelbrot.c
	$(CC) $(CFLAGS) -c $<

buddhabrot.exe: buddhabrot.o image.o spinner.o common.o
	$(CC) -o $@ $^ $(LFLAGS)

buddhabrot.o: buddhabrot.c
	$(CC) $(CFLAGS) -c $<

buddhabrotpp.exe: buddhabrotpp.o image.o spinner.o common.o
	$(CXX) -o $@ $^ $(LFLAGS)

buddhabrotpp.o: buddhabrotpp.cpp
	$(CXX) $(CXXFLAGS) -c $<

buddhabrotmt.exe: buddhabrotmt.o image.o common.o
	$(CXX) -o $@ $^ $(LFLAGS)

buddhabrotmt.o: buddhabrotmt.cpp
	$(CXX) $(CXXFLAGS) -c $<

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
	rm -f *.o $(OBJS)
