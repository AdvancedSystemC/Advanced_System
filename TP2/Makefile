CC = gcc
CFLAGS = -Wall -Wextra

SRC_DIR = src
BIN_DIR = bin

EXE1 = $(BIN_DIR)/Exo1
EXE2 = $(BIN_DIR)/Exo2
EXE3 = $(BIN_DIR)/Exo3

SRCS1 = $(wildcard $(SRC_DIR)/Exo1.c)
SRCS2 = $(wildcard $(SRC_DIR)/Exo2.c)
SRCS3 = $(wildcard $(SRC_DIR)/Exo3.c)

OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)
OBJS3 = $(SRCS3:.c=.o)

.PHONY: all clean

all: $(EXE1) $(EXE2) $(EXE3)

$(EXE1): $(OBJS1)
	$(CC) $(CFLAGS) $^ -o $@

$(EXE2): $(OBJS2)
	$(CC) $(CFLAGS) $^ -o $@

$(EXE3): $(OBJS3)
	$(CC) $(CFLAGS) $^ -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS1) $(OBJS2) $(OBJS3) $(EXE1) $(EXE2) $(EXE3)