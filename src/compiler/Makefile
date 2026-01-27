CXX := $(shell which clang++ 2>/dev/null || which g++ 2>/dev/null || echo c++)
CC := $(shell which clang 2>/dev/null || which gcc 2>/dev/null || echo cc)
LLVM_CONFIG := llvm-config
LLVM_CXXFLAGS := $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags --system-libs --libs core native analysis)
CPPFLAGS := -I..

BIN_DIR := ../../bin

CPP_SOURCES := main.cpp ast.cpp codegen.cpp sema.cpp types.cpp
C_SOURCES := Absyn.c Buffer.c Lexer.c Parser.c Printer.c
CPP_OBJECTS := $(CPP_SOURCES:.cpp=.o)
C_OBJECTS := $(C_SOURCES:.c=.o)

TARGET_NAME := dory

ifeq ($(OS),Windows_NT)
EXE_EXT := .exe
else
EXE_EXT :=
endif

all: dirs $(BIN_DIR)/$(TARGET_NAME)$(EXE_EXT)

dirs:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(TARGET_NAME)$(EXE_EXT): $(CPP_OBJECTS) $(C_OBJECTS) FORCE
	$(CXX) -O2 -Wall -Wextra -o $@ $(filter-out FORCE,$^) $(LLVM_LDFLAGS)

%.o: %.cpp FORCE
	$(CXX) -O2 -Wall -Wextra -Wno-extern-c-compat $(LLVM_CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.c FORCE
	$(CC) -O2 -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable $(CPPFLAGS) -c $< -o $@

FORCE:

clean:
	rm -f $(BIN_DIR)/$(TARGET_NAME)$(EXE_EXT)
	rm -f $(CPP_OBJECTS) $(C_OBJECTS)

.PHONY: all clean dirs FORCE
