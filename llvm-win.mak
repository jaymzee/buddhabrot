# gnu makefile targetting LLVM clang compiler for windows
# buddhabrot omitted from OBJS because clang on windows imports the
# MSVC headers which do not support the double complex type

CC = clang
CXX = clang++
CFLAGS = -O2 -D_CRT_SECURE_NO_WARNINGS -std=c99 -pedantic -Werror
CXXFLAGS = -O2 -D_CRT_SECURE_NO_WARNINGS -std=c++17 -pedantic -Werror
LFLAGS = 
TARGETS = mandelbrot.exe buddhabrot.exe buddhabrotpp.exe buddhabrotmt.exe

all: $(TARGETS)

mandelbrot.exe: obj/mandelbrot.o
	$(CC) -o $@ $^ $(LFLAGS)

obj/mandelbrot.o: src/mandelbrot.c
	$(CC) -o $@ $(CFLAGS) -c $<

buddhabrot.exe: obj/buddhabrot.o obj/image.o obj/spinner.o obj/common.o
	$(CC) -o $@ $^ $(LFLAGS)

obj/buddhabrot.o: src/buddhabrot.c
	$(CC) -o $@ $(CFLAGS) -c $<

buddhabrotpp.exe: obj/buddhabrotpp.o obj/image.o obj/spinner.o obj/common.o
	$(CXX) -o $@ $^ $(LFLAGS)

obj/buddhabrotpp.o: src/buddhabrotpp.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $<

buddhabrotmt.exe: obj/buddhabrotmt.o obj/spinner.o obj/myrandom.o \
		  obj/image.o obj/common.o
	$(CXX) -o $@ $^ $(LFLAGS)

obj/buddhabrotmt.o: src/buddhabrotmt.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $<

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
