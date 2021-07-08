This program was developed starting July 14, 2020, to parse some number of (ASCII) files for duplicate lines and create numbered output files with the duplicate lines removed. Program developed for a Windows system.

The program can either take its arguments from the command line or from standard input.

Accepted inputs:
	any list of file names with or without their associated path

Accepted options:
	-h	displays help
	-i	treats the next argument as if it were a file name (rather than as any option flag)
	-n	makes the line delimiter in subsequent files the newline character (which is the default behavior)
	-r	changes the line delimiter in subsequent files to carriage return (instead of the default newline character)
	-v	toggles whether to additionally print (to standard output) the duplicate lines found for subsequent each file and how many of each were removed (verbose mode is off by default)
