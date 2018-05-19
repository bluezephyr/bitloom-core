[![Build Status](https://travis-ci.org/bluezephyr/bitloom-core.svg?branch=master)](https://travis-ci.org/bluezephyr/bitloom-core)

# BitLoom Core
This repository contains the Core parts of the Bit Loom project. It includes a simple non-preemptive scheduler that will run run functions contained in tasks. The scheduler is run using a timer that has to be provided from outside. Typically the timer shall run the scheduler once per millisecond or similar.

# Unit Tests
The project includes a set of unit tests. The tests use the Unity (http://www.throwtheswitch.org/unity) test framework. Make sure to download the test framework from https://github.com/ThrowTheSwitch/Unity and set the environment variable UNITY_HOME. 

On Linux use the following sequence:

    export UNITY_HOME=<path to Unity folder>
    cd bitloom-core/tests
    make

Unit tests are executed on each commit by Travis CI: https://travis-ci.org/bluezephyr/bitloom-core

