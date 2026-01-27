CC := $(shell which clang 2>/dev/null || which gcc 2>/dev/null || echo cc)

BIN_DIR := ../../bin

TARGET_NAME := domk2

ifeq ($(OS),Windows_NT)
EXE_EXT := .exe
else
EXE_EXT :=
endif

all: dirs $(BIN_DIR)/$(TARGET_NAME)$(EXE_EXT)

CFLAGS := -O2 -Wall -Wextra -Werror=unused-variable

dirs:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(TARGET_NAME)$(EXE_EXT): domk2.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BIN_DIR)/$(TARGET_NAME)$(EXE_EXT)

.PHONY: all clean dirs
