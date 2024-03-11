# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -Wall -Werror -std=c99

trans.o: trans.c
	$(CC) $(CFLAGS) -O0 -c trans.c

# Clean up intermediate files and executable
clean:
	rm -rf *.o