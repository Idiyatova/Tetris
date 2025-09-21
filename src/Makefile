CC = gcc
DB_DIR = database
DB_FILE = $(abspath $(DB_DIR)/tetris.db)
CFLAGS = -Wall -Wextra -Werror -std=c11 -DDB_PATH='"$(DB_FILE)"'
LDFLAGS = -lncurses -lsqlite3

SRC_DIR = tetris
GUI_DIR = tetris/cli
TEST_DIR = $(SRC_DIR)/tests
BUILD_DIR = build
REPORT_DIR = report

BIN = $(BUILD_DIR)/tetris
LIB = $(BUILD_DIR)/libtetris.a
TEST_BIN = $(BUILD_DIR)/test

.PHONY: all clean test gcov_report dvi dist run

ifeq ($(OS), Darwin)  
    CHECK_LIBS = -lcheck
    OPEN_CMD = open
else 
    CHECK_LIBS = -lcheck -lsubunit -lm -lrt -lpthread
    OPEN_CMD = xdg-open
endif

all: clean $(BIN)

$(DB_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

$(REPORT_DIR):
	mkdir -p $@

$(LIB): $(wildcard $(SRC_DIR)/*.c) | $(BUILD_DIR) $(DB_DIR)
	$(CC) $(CFLAGS) -c $^ -I$(SRC_DIR)/inc
	ar rcs $@ *.o
	rm -f *.o

$(BIN): $(LIB) $(wildcard $(GUI_DIR)/*.c)
	$(CC) $(CFLAGS) -c $(GUI_DIR)/*.c -I$(SRC_DIR)/inc -I$(GUI_DIR)
	$(CC) $(CFLAGS) *.o -L$(BUILD_DIR) -ltetris $(LDFLAGS) -o $@
	rm -f *.o

test: $(wildcard $(SRC_DIR)/*.c) $(wildcard $(TEST_DIR)/*.c) | $(BUILD_DIR)
	$(CC) $(CFLAGS) --coverage $^ -I$(SRC_DIR)/inc -I$(TEST_DIR) \
	$(LDFLAGS) $(CHECK_LIBS) -o $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: test | $(REPORT_DIR)
	lcov --capture --directory . --output-file $(REPORT_DIR)/coverage.info --rc geninfo_unexecuted_blocks=1
	genhtml $(REPORT_DIR)/coverage.info -o $(REPORT_DIR)
	$(OPEN_CMD) $(REPORT_DIR)/index.html 


dist: clean all dvi 
	mkdir -p $(DIST_DIR)
	cp -r $(SRC_DIR) $(GUI_DIR) Makefile $(DOC_DIR)/manual.dvi $(DIST_DIR)/
	tar -czvf tetris.tar.gz -C $(DIST_DIR) .
	rm -rf $(DIST_DIR)


clean:
	rm -rf coverage.info \
	rm -rf $(BUILD_DIR) $(DB_DIR) $(REPORT_DIR) \
	$(DOC_DIR)/*.aux $(DOC_DIR)/*.log $(DOC_DIR)/*.dvi $(DOC_DIR)/*.out $(DOC_DIR)/*.toc \
	tetris.tar.gz
	find . -name "*.gcda" -o -name "*.gcno" -delete

