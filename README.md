# MyCLib

[Read in Portuguese](README.pt-br.md) | [中文版](README.zh-cn.md)

## Important Notice

> **This project is for learning purposes only.**
> 
> All files in `src/` and `include/` were written **manually without AI assistance**, with the **exception of `ui.h`, `ui.c`, and `main.c`**, which were created using AI agents to provide a visual interface for demonstrating the backend functionality.

---

**Objective:** To show my knowledge about the C language and its particularities.

This project is a collection of my own implementations of libraries and algorithms, aiming to understand how things work under the hood.

You can compile the project using **CMake**:

```bash
mkdir build && cd build
cmake ..
make
./main
```

## Functions

### String Utils

*   **my_strlen**: Returns the length of a string using pointer arithmetic.
*   **my_tolower**: Converts a string to lowercase (returns a new allocated string).
*   **my_toupper**: Converts a string to uppercase (returns a new allocated string).
*   **my_strcmp**: Compares two strings lexicographically. Returns `0` if equal, `1` if str1 is larger, `-1` if str2 is larger.
*   **my_strcmp_percent**: Calculates string similarity using Levenshtein distance algorithm. Returns a percentage (0-100).

### Math Utils

*   **min**: Returns the minimum of three integers (used by `my_strcmp_percent` for dynamic programming).
