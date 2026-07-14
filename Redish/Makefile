CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -g
BUILD   := build

.PHONY: all redish test coverage clean

all: redish

# Build the interactive Redish CLI.
redish: hello_world.c store.c store.h
	$(CC) $(CFLAGS) hello_world.c store.c -o $(BUILD)/redish

# Build and run the unit test suite.
test: store.c store.h tests/test_store.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I. store.c tests/test_store.c -o $(BUILD)/test_store
	./$(BUILD)/test_store

# Run the tests with gcov instrumentation and print a line-coverage report.
coverage: store.c store.h tests/test_store.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) --coverage -I. -c store.c -o $(BUILD)/store.o
	$(CC) $(CFLAGS) --coverage -I. -c tests/test_store.c -o $(BUILD)/test_store.o
	$(CC) $(CFLAGS) --coverage $(BUILD)/store.o $(BUILD)/test_store.o -o $(BUILD)/test_store_cov
	./$(BUILD)/test_store_cov
	gcov -o $(BUILD) store.c

$(BUILD):
	mkdir -p $(BUILD)

redish: | $(BUILD)

clean:
	rm -rf $(BUILD)
	rm -f *.gcno *.gcda *.gcov
