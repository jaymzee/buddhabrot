
# Release notes 1.2.1
## changes since 1.2.0
* fixed bug that could cause buddhabrotmt to wait indefinately for threads
  to finish if samples is not evenly divisible by the number of threads

# Release notes 1.2.0
## changes since 1.1.0

### features added:
* **buddhabrotmt** multithreaded implementation
* -t number of worker threads option

# Release notes 1.1.0
## changes since 1.0.0

### features added:
* -d random seed option
* -v version option
* provide output filename directly (instead redirecting stdout)

### other changes:
* better error handling of command line options
* better compiler support for buddhabrot (C implementation)
* *max iterations* and *samples* accept 64 bit integers
* mplshow and pilshow use -? style command line options
