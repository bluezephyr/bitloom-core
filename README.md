# BitLoom Core
This repository contains the Core parts of the Bit Loom project. It includes a simple non-preemptive scheduler that will run run functions contained in tasks. The scheduler is run using a timer that has to be provided from outside. Typically the timer shall run the scheduler once per millisecond or similar.
