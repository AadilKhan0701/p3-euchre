# p3-euchre

EECS 280 Project 3 (Euchre) implementation in C++.

## Contents

- `Card.hpp` / `Card.cpp`: card representation and comparison logic
- `Pack.hpp` / `Pack.cpp`: Euchre deck handling
- `Player.hpp` / `Player.cpp`: player strategy implementations
- `euchre.cpp`: main game program
- `*_public_tests.cpp` and `*_tests.cpp`: public and custom tests
- `Makefile`: build, test, style, and clean commands

## Build

```bash
make
```

This builds the test executables and `euchre.exe`.

## Run tests

```bash
make test
```

## Run the game

Example invocation:

```bash
./euchre.exe pack.in noshuffle 1 Alice Simple Bob Simple Carol Simple Dave Simple
```

## Clean generated files

```bash
make clean
```
