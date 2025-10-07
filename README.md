# LSH – Shaurya Tamang's Mini Shell

LSH is a simple Unix/Linux command-line shell written in C. It demonstrates core concepts of shell programming, including process creation, execution, built-in commands, and token parsing.

---

## Features

- Execute **external programs** using `fork()` and `execvp()`.
- Supports **built-in commands**:
  - `cd` – change directory
  - `help` – display built-in commands
  - `exit` – exit the shell
- Tokenizes user input into commands and arguments.
- Handles **basic error cases** (invalid commands, `chdir` errors, memory allocation errors).

---

## Requirements

- GCC or any C compiler
- Unix/Linux operating system

---

## Installation

1. Clone or download the repository.
2. Compile using GCC:

```
gcc -o main main.c
```
3. Run the shell:
```
./main
```

---

## Usage

```
$ help
$ cd /path/to/directory
$ ls -l
$ exit
```
* Type commands like in a normal terminal.
* Built-in commands (`cd`, `help`, `exit`) are handled internally.

---

## Author

 **Shaurya Tamang**  – Built this shell as a learning project to understand processes, memory management, and function pointers in C.