CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-I ./include/

SRC_DIR=./src
INC_DIR=./include
BIN_DIR=./bin
DOC_DIR=./doc
GCOV_DIR=./gcov

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g

LCOV_REPORT=report.info

REVERSE_SRC=$(wildcard $(SRC_DIR)/*.c)
LS_SRC=$(wildcard $(SRC_DIR)/*.c)
COPY_SRC=$(wildcard $(SRC_DIR)/*.c)

REVERSE_OBJ=$(REVERSE_SRC:.c=.o)
LS_OBJ=$(LS_SRC:.c=.o)
COPY_OBJ=$(COPY_SRC:.c=.o)

REVERSE_EXEC=reverse
LS_EXEC=ls
COPY_EXEC=copy

G_REVERSE_EXEC=$(REVERSE_EXEC).cov
G_LS_EXEC=$(LS_EXEC).cov
G_COPY_EXEC=$(COPY_EXEC).cov

AR_NAME=archive_programs.tar.gz

all: reverse ls copy

reverse: $(REVERSE_SRC) $(REVERSE_EXEC)
ls: $(LS_SRC) $(LS_EXEC)
copy: $(COPY_SRC) $(COPY_EXEC)

%.o:%.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(REVERSE_EXEC): $(REVERSE_OBJ)
	$(CC) -o $(BIN_DIR)/$@ -Wall $(LDFLAGS) $(REVERSE_OBJ)

$(LS_EXEC): $(LS_OBJ)
	$(CC) -o $(BIN_DIR)/$@ -Wall $(LDFLAGS) $(LS_OBJ)

$(COPY_EXEC): $(COPY_OBJ)
	$(CC) -o $(BIN_DIR)/$@ -Wall $(LDFLAGS) $(COPY_OBJ)

$(G_REVERSE_EXEC):
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ -Wall $(LDFLAGS) $(REVERSE_SRC)

$(G_LS_EXEC):
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ -Wall $(LDFLAGS) $(LS_SRC)

$(G_COPY_EXEC):
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ -Wall $(LDFLAGS) $(COPY_SRC)

doc:
	doxygen $(DOC_DIR)/doxygen.conf

gcov: $(G_REVERSE_EXEC) $(G_LS_EXEC) $(G_COPY_EXEC)
	# Generate some data for gcov by calling the generated binaries with various options
	$(GCOV_DIR)/$(G_REVERSE_EXEC) -h
	$(GCOV_DIR)/$(G_REVERSE_EXEC) -i input -o output -v
	$(GCOV_DIR)/$(G_LS_EXEC) -h
	$(GCOV_DIR)/$(G_LS_EXEC) -i input -o output -v
	$(GCOV_DIR)/$(G_COPY_EXEC) -h
	$(GCOV_DIR)/$(G_COPY_EXEC) -i input -o output -v

	find ./ -maxdepth 1 -name *.gcno -exec mv {} $(GCOV_DIR) \;
	find ./ -maxdepth 1 -name *.gcda -exec mv {} $(GCOV_DIR) \;

	gcov -o $(GCOV_DIR) $(G_REVERSE_EXEC)
	gcov -o $(GCOV_DIR) $(G_LS_EXEC)
	gcov -o $(GCOV_DIR) $(G_COPY_EXEC)

	lcov -o $(GCOV_DIR)/$(LCOV_REPORT) -c -f -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/$(LCOV_REPORT)

package: gcov doc all
	rm -rf $(AR_NAME)
	tar cvfz $(AR_NAME) ./*

clean:
	rm -rf $(REVERSE_OBJ) $(LS_OBJ) $(COPY_OBJ)

mrproper: clean
	rm -rf $(BIN_DIR)/*
	rm -rf $(DOC_DIR)/latex/
	rm -rf $(DOC_DIR)/html/
	rm -rf $(GCOV_DIR)/*

.PHONY: doc reverse ls copy