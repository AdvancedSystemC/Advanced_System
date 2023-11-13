CC = gcc
CFLAGS = -Wall -Wextra

SRC_DIR = src
BIN_DIR = bin

EXE1 = $(BIN_DIR)/compteur

SRCS1 = $(wildcard $(SRC_DIR)/compteur.c)

OBJS1 = $(SRCS1:.c=.o)

.PHONY: all clean

all: $(EXE1)

$(EXE1): $(OBJS1)
	$(CC) $(CFLAGS) $^ -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS1) $(EXE1) 