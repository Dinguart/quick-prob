CC = gcc
FLAGS = -Wall -Wextra -pedantic
CFLAGS = $(FLAGS) -g -O2 -Iinclude
LFLAGS = -lm

SRC = prob.c
OBJ = $(SRC:.c=.o)

prob: $(OBJ)
	$(CC) $(OBJ) -o prob $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


