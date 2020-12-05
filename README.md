[![Build Status](https://travis-ci.org/bluezephyr/bitloom-core.svg?branch=master)](https://travis-ci.org/bluezephyr/bitloom-core)

# BitLoom core

This repository contains the Core parts of the Bit Loom project. It includes a
simple non-preemptive scheduler that will run run functions contained in tasks.
The scheduler is run using a timer that has to be provided from outside.
Typically the timer shall run the scheduler once per millisecond or similar.

## Build

BitLoom core is built using [CMake](https://cmake.org).

## Unit tests

The project includes a set of unit tests. The tests use the CppUTest test framework
(see http://cpputest.github.io/). To build the tests, the CMake variable
COMPILE_TESTS must be set (-DCOMPILE_TESTS=ON). The build scripts will use the
Cpputest binaries on the path. Use the CMake variable CPPUTEST_HOME to specify
an alternative loccation (-DCPPUTEST_HOME=\<path to CppUTest>).

### Linux unit test build

On Linux use the following sequence to generate makefiles, build and run the
tests:

    mkdir build
    cd build
    cmake -DCOMPILE_TESTS=ON -DCPPUTEST_HOME=<path to CppUTest> ..
    make all
    make test

# Continuous integration

Unit tests are executed on each commit by
[Travis CI](https://travis-ci.org/bluezephyr/bitloom-core).

