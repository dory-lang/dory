# Dory

Dory is an experimental compiler being created with simple syntax and easy access to C libraries (Language Bindings) in mind.
It is inspired by C89/C90 and [xBase](https://en.wikipedia.org/wiki/XBase). Dory is dynamically typed, has a simple syntax, and its own compiler based on [LLVM](https://www.llvm.org/).
The repo also includes a runtime/VM and helper tools.

## What exists now
- syntax front-end (BNFC) + AST
- semantic analysis (basic checks)
- LLVM backend (IR and object files)
- runtime/VM (basic functions and types)

## Project structure
- `src/compiler/` - compiler (parser, sema, codegen)
- `src/rtl/` - runtime/VM and standard functions
- `src/domk2/` - helper tool
- `tests/` - language examples and tests
- `bin/`, `lib/` - build artifacts

## Requirements
- MSYS2 (CLANG64) on Windows
- Linux (clang/llvm-config + make)
- LLVM 15+ and clang/llvm-config
- bison, flex (for BNFC)

## Building
From the project root directory:

```sh
make
```

Runtime separately:

```sh
make -C src/rtl
```

## Compiler usage
Examples:

```sh
dory tests/stmt_if.dor --dump

dory tests/stmt_if.dor --bin stmt_if
./stmt_if.exe
```

Most important options:
- `--dump` - AST dump
- `--ast` - pretty print AST
- `--ir` / `--ir-out <path>` - LLVM IR
- `--obj <path>` - object file
- `--bin <path>` - linking into a binary

## Status
For now this is a kind of digital construction site. Not all statements and types are supported yet.
If compilation succeeds but running returns `error: codegen failed`, it means that the given construct does not have support in the backend yet.
