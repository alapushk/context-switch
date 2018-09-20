-----------------
**** README ****
-----------------
 
Files included:
- this README.txt
- Makefile
- source file a2.c
- source file hr-timer.c with my annotations
 
—————————————
Instructions
—————————————
 
In order to execute this program, type "make" in the command line.
Then type “./a2”.

———————————
Description
———————————
 
This program measures minimal function call cost, minimal system call cost, process switching cost on LINUX machines.

——————————————
Implementation
—————————————— 

I implemented my program in C language. When thinking of the structure of my program, I decided to split the main functionality into several functions. I have a separate function for measuring the cost of a minimal function call (function_call), measuring the cost of a minimal system cal (system_call), measuring the cost of a process switching (process_switch). My main function contains declaration of some variables and the function calls.

——————————
Resources
——————————

I visited these websites in order to see some examples of C functions and Linux manual pages:
	- https://linux.die.net/man/
	- https://www.tutorialspoint.com/cprogramming
	- http://tldp.org/LDP/lpg/node1.html
	- https://www.gnu.org/software/libc/manual/html_node/index.html#SEC_Contents

Created by Alina Lapushkina on 2017-02-10.
Copyright (c) 2017 Alina Lapushkina. All rights reserved.
