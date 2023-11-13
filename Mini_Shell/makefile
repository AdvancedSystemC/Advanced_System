CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-I ./include/

SRC_DIR=./src
INC_DIR=./include
OBJ_DIR=./obj
BIN_DIR=./bin
DOC_DIR=./doc
GCOV_DIR=./gcov

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g

LCOV_REPORT=report.info

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
INC_FILES=$(wildcard $(INC_DIR)/*.h)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

EXEC=my_sh

GEXEC=$(EXEC).cov

AR_NAME=archive_$(EXEC).tar.gz

all: $(OBJ_DIR) $(BIN_DIR) $(SRC_FILES) $(EXEC)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(EXEC): $(OBJ_FILES)
	$(CC) -o $(BIN_DIR)/$@ -Wall $(LDFLAGS) $(OBJ_FILES)

$(GEXEC):
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ -Wall $(LDFLAGS) $(SRC_FILES)

doc:
	doxygen $(DOC_DIR)/doxygen.conf

gcov: $(GEXEC)
	# Generate some data for gcov by calling the generated binary with various options
	$(GCOV_DIR)/$(GEXEC) -h
	$(GCOV_DIR)/$(GEXEC) -i input -o output -v

	find ./ -maxdepth 1 -name *.gcno -exec mv {} $(GCOV_DIR) \;
	find ./ -maxdepth 1 -name *.gcda -exec mv {} $(GCOV_DIR) \;

	gcov -o $(GCOV_DIR) $(GEXEC)
	lcov -o $(GCOV_DIR)/$(LCOV_REPORT) -c -f -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/$(LCOV_REPORT)

package: gcov doc all
	rm -rf $(AR_NAME)
	tar cvfz $(AR_NAME) ./*

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/*

mrproper: clean
	rm -rf $(DOC_DIR)/latex/
	rm -rf $(DOC_DIR)/html/
	rm -rf $(GCOV_DIR)/*