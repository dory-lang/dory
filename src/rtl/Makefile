CC := $(shell which clang 2>/dev/null || which gcc 2>/dev/null || echo cc)
AR := ar

LIB_DIR := ../../lib

C_SOURCES := $(wildcard *.c)
C_OBJECTS := $(C_SOURCES:.c=.o)

LIBNAME := libdory.a

all: dirs $(LIB_DIR)/$(LIBNAME)

CFLAGS := -O2 -Wall -Wextra -Werror=unused-variable -I..

dirs:
	@mkdir -p $(LIB_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/$(LIBNAME): $(C_OBJECTS)
	$(AR) rcs $@ $^
	rm -f $(C_OBJECTS)

clean:
	rm -f $(C_OBJECTS) $(LIB_DIR)/$(LIBNAME)

.PHONY: all clean dirs
