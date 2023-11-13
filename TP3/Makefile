CC = gcc
CFLAGS = -Wall -Wextra -pthread
SRC_DIR = src
BIN_DIR = bin
EXECUTABLE = Exo1

all: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(SRC_DIR)/$(EXECUTABLE).c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BIN_DIR)/$(EXECUTABLE)
