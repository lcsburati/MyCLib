# MyCLab

[English](README.md) | [Português](README.pt-br.md) | [中文版](README.zh-cn.md)

## Index
- [Purpose](#purpose)
- [How to compile (CMake)](#how-to-compile-cmake)
- [Important Notice](#important-notice)
- [Functions](#functions)
  - [`string_utils.h`](#string_utilsh)
  - [`string_utils.c`](#string_utilsc)

## Purpose

**Purpose:** The project aims to understand the behavior of the C language through simulations of some libraries, based on my interpretation. The idea is to achieve similar results using my current knowledge, focusing on understanding the language syntax, its data structures, among other aspects. The purpose is to truly understand how low-level languages work (in this case, C), understanding the abstractions that happen behind the scenes. Additionally, I want to deepen my knowledge and practice my logical reasoning to improve the way I solve problems.

## How to compile (CMake)

You can compile the project using **CMake**:

```bash
mkdir build && cd build
cmake ..
make
./main
```

## Important Notice

> All files in `src/` and `include/` were written **manually without AI assistance**, with the **exception of `ui.h`, `ui.c`, and `main.c`**, which were created using AI agents to provide a visual interface to demonstrate the backend functionalities.

## Functions

### string_utils.h

> Here the goal is to store prototypes, structures, and constants to keep the source string_utils.c cleaner.

* `#define OFFSET ('a' - 'A')`: constant that defines the distance between lowercase and UPPERCASE (`32`). Used in my_tolower and my_toupper.

* `#define STR_BUFFER 256`: defines the maximum character size a string can have.

* OBS: The choice of #define instead of const was due to the simplicity of the variables, since typing will not be important at runtime.

* `typedef char * string`: defines an alias for char*. We could have worked directly with pointers, but to facilitate understanding of how strings work under the hood, I chose to use the alias string for char*. (I understand that hiding pointers in typedefs is not a good practice in C, but in this case it was only to ensure the understanding that strings are naturally pointers).

### string_utils.c

> This is the pair of string_utils.h and is where the logic of the prototyped functions defined in the header will be applied.

<details>
<summary><strong>my_strlen</strong> (click to expand)</summary>

In this function, the input parameter is a `string` type (`char*` — pointer to `char`).  
The function return type is `size_t`, chosen to facilitate portability regarding the size of an unsigned int, since we are dealing with the size of arrays (strings).

We start by defining the variable that will count the string (array) length, named here as `length`, also of type `size_t`, for the same reason as the function return (in addition to the fact that it will be precisely the value returned).

The logic from there is simple: knowing that every string in C ends with the null character `\0`, we can traverse the string until we find it. This character is interpreted as `0` or `false` (since booleans do not exist natively in C), so we can simply use `while (*str)` to indicate to the program that it should traverse the pointer starting at the input string until it finds the `\0`. With each loop iteration, we increment `length` by 1, accumulating the string size.

**Invalid input and overflow check:** The function verifies if the string is `NULL`; if so, it immediately returns `0`. Additionally, if during iteration the length reaches or exceeds `STR_BUFFER`, the function returns `STR_BUFFER + 1`. I did this to differentiate an error/overflow from a valid count, since returning `0` could also represent an empty string. The `+ 1` helps handle edge cases and makes it clearer to the functions that depend on `my_strlen` that the size went past the expected limit.

**OBS:** The function recognizes spaces (` `) as a character as well. That is, if you pass a string with spaces, the returned size will be the total string size, including spaces. I think it's important to mention this, because depending on the context some functions/approaches treat space as a separator and end up "ignoring" this character (for example, when reading input with `scanf("%s")`).

#### Usage Example

The `my_strlen` function is in the sidebar, in the **STRING_UTILS** section, as shown in the image below:

![my_strlen function location in sidebar](./assets/my_strlen_exemple.png)

After selecting `my_strlen`, you can enter a string and view the calculated length:

![my_strlen usage example](./assets/my_strlen_exemple2.png)

</details>

<details>
<summary><strong>my_toupper</strong> (click to expand)</summary>

In this function, the input parameter is a string (char pointer) named `str`.

We start by defining the necessary variables for processing: `length` (`size_t`) and `result`. For `result`, we allocate memory on the *heap* through the `malloc` function, creating a pointer that marks the beginning of a block with the exact size of the input string + 1 byte of space for the null character `\0`.

After allocation, we verify that `malloc` was successful; otherwise, the function returns `NULL` (preventing failures if there is no contiguous space in memory for block allocation).

We also define an auxiliary pointer `string current = result`. This allows us to traverse and fill the new memory block without losing the reference to the beginning of the block (`result`), which will be necessary for the function return.

The conversion logic traverses the input string (`str`): we check if the current character is a lowercase letter; if so, we convert to uppercase by subtracting the `OFFSET` value (32) from the ASCII value of the character. Otherwise, we simply copy the original character to the new block.

At the end of the loop, we set the last character of `current` to `\0`, correctly terminating the string. We return `result`, which points to the beginning of the new string now in **UPPERCASE**.

**Empty input or buffer limit check:** The function verifies if the string length is `0` or greater than `STR_BUFFER`; if so, it returns `NULL` to avoid unnecessary allocations and also prevent a string marked as overflow by `my_strlen` from continuing into allocation.

</details>

<details>
<summary><strong>my_tolower</strong> (click to expand)</summary>

In this function, we follow the same memory management logic as `my_toupper`, using an input pointer `str` and allocating a new block on the *heap* with `malloc` to store the result.

The fundamental difference lies in the conversion logic: we traverse the input string checking if the current character is an **uppercase** letter. If it is, we convert to **lowercase** by **adding** the `OFFSET` value (32) to the ASCII value of the character.

As with other functions, we maintain security by checking the success of memory allocation. At the end, we ensure the string is terminated with the null character `\0` and return the `result` pointer, which points to the beginning of the new string converted to **lowercase**.

**Empty input or buffer limit check:** The function verifies if the string length is `0` or greater than `STR_BUFFER`; if so, it returns `NULL` to avoid unnecessary allocations and also prevent a string marked as overflow by `my_strlen` from continuing into allocation.

</details>

<details>
<summary><strong>my_strcmp</strong> (click to expand)</summary>

The input parameters are two strings to be compared: `str1` and `str2`. The idea here is to simulate the behavior of `strcmp`: if both strings are equal, the function returns `0`; if they are different, the return value comes from the difference between the first different characters found.

First, we validate whether either input is null. If `str1` or `str2` is `NULL`, the function returns `1`, indicating that the comparison is not equal.

After that, the main processing starts: while neither string has reached the null character `\0`, the loop remains valid. Inside the loop, we compare the current character from `str1` with the current character from `str2`. If they are different, the function immediately returns the difference between those two characters. If they are equal, both pointers move forward with `str1++` and `str2++`, going to the next character in each string.

When the loop ends, we return the difference between the current characters one more time. This covers the case where one string ends before the other, for example `"abc"` and `"ab"`: at that point, the comparison is between `'c'` and `'\0'`.

The return expressions use a type cast to `unsigned char` because we are using `char` values inside integer operations. Depending on the compiler, plain `char` can be interpreted as signed or unsigned, which can affect characters outside the basic ASCII table. By converting to `unsigned char`, the comparison becomes more predictable and closer to the expected behavior of `strcmp`.

**Return codes:**

- `0`: the strings are equal.
- Negative value: the first different character in `str1` is smaller than the one in `str2`.
- Positive value: the first different character in `str1` is greater than the one in `str2`.
- `1` during the initial check: at least one input is `NULL`.

</details>

<details>
<summary><strong>my_strcmp_percent</strong> (click to expand)</summary>

The input parameters are `str1` and `str2`, similar to `my_strcmp`. The main difference lies in the algorithm: to calculate the similarity percentage, we use the **Levenshtein distance**, which fills a matrix based on the minimum number of operations needed to transform `str2` into `str1` — these operations being: **insertion**, **substitution**, and **deletion** of characters.

The algorithm starts by defining the sizes of `str1` and `str2` using `my_strlen`. Since `my_strlen` now returns `STR_BUFFER + 1` when it detects overflow, `my_strcmp_percent` uses that information to stop before trying to build the Levenshtein matrix with an invalid size.

Before moving to the main calculation, the function handles a few special return values: if either input is `NULL`, it returns `-3`; if either string goes past `STR_BUFFER`, it returns `-2`; and if any memory allocation fails, it returns `-1`.

After these checks, the function allocates the `matrix` on the heap — treated as a double pointer (`int**`) — using `malloc` for both rows and columns via for loop. There is also a protection for failure in the middle of row allocation: if any column cannot be allocated, the function frees everything that had already been reserved before returning the error, preventing memory leaks.

The first two loops after allocation define the **matrix borders**: the left border (column 0) and the top border (row 0) are filled with increasing values, representing the cost of starting from zero — that is, how many operations would be needed if one of the strings were empty.

In the main loop (two nested loops traversing rows and columns), we define the `cost` variable via ternary conditional: if the compared characters are equal, `cost = 0`; otherwise, `cost = 1`.

Then, we take the **minimum among the 3 operations** — because we want the path of least cost to reach the result, since it is possible to transform one string into another in different ways, but with different costs. Each operation is represented by one of the 3 adjacents of the current cell:

- **`[i-1][j]` → deletion:** we advance only in one direction (str1), meaning that character should be ignored — cost +1.
- **`[i][j-1]` → insertion:** we advance only in the direction of str2, that is, the string we are comparing is missing 1 character — cost +1.
- **`[i-1][j-1]` → substitution (or match):** we go back one in both directions, keeping synchronization — cost +1 if characters are different, or 0 if they are equal.

With the complete loop, the matrix is fully filled and the value at `matrix[str1_length][str2_length]` is the final accumulated distance of all operations.

From there, we use a ternary conditional to identify which string is larger and use its size as the denominator in the percentage calculation. If both strings are empty, that denominator becomes `0`; in that case, the function frees the matrix and returns `-4`, avoiding division by zero. Finally, in the normal flow, we free the allocated matrix space and return the similarity as `double`.

**Return codes:**

- `0` to `100`: similarity percentage calculated successfully.
- `-1`: matrix memory allocation failed.
- `-2`: at least one string went past `STR_BUFFER`, indicating overflow detected by `my_strlen`.
- `-3`: at least one input is `NULL`.
- `-4`: both strings are empty, leaving the calculation denominator equal to `0`.

**OBS about empty strings:** Since an empty string does not go past `STR_BUFFER`, one empty string compared with another string that has content can still continue to the calculation and tends to return `0%`. The case where both strings are empty is handled separately with `-4`, because there is no maximum length to use as the percentage base.

**OBS:** The error codes are now negative exactly to avoid confusing them with valid percentages. Since the normal return stays between `0` and `100`, any negative value means the function stopped before the final calculation.

</details>
