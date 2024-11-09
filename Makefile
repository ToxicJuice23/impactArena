# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O3 -I./src
TESTFLAGS = -Wno-format-extra-args -Wall -Wextra -O3 -I./src
LDFLAGS = $(shell sdl2-config --cflags --libs) -lm

# Source and target
HEADERS = src/main.h
TARGET = build/impactArena
DEBUG_OBJECT = build_debug/impactDebug.o
DEBUG_TARGET = build_debug/impactDebug
SRC = src/ball_physics.c src/string_functions.c src/main.c
TEST_SRC = tests/check_ball.c src/ball_physics.c src/string_functions.c
TEST_LIBS = -lcheck -lsubunit
TEST_TARGET = tests/testGame

# Default target
all: test impactArena
debug: $(DEBUG_TARGET)

impactArena: $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(LDFLAGS)

test: $(SRC) $(TEST_SRC)
	#checkmk tests/check_ball.check.c > tests/check_ball.c
	$(CC) $(TESTFLAGS) -o $(TEST_TARGET) $(TEST_SRC) $(TEST_LIBS) $(LDFLAGS)

# Build target
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

$(DEBUG_TARGET): $(SRC) $(HEADERS)
	mkdir -p build_debug
	$(CC) $(CFLAGS) -g -c -o $(DEBUG_OBJECT) $(SRC) $(LDFLAGS)
	$(CC) $(CFLAGS) -g -o $(DEBUG_TARGET) $(SRC) $(LDFLAGS)

# Clean target
clean:
	rm -f ${TEST_TARGET}
	rm -rf build build_debug