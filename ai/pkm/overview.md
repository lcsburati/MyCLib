# Overview

## What is MyCLab

An educational C99 project that recreates standard C library functions (string.h, stdlib.h) from scratch, aiming to understand how the language works under the hood.

## Motivation

- Understand low-level abstractions
- Deepen problem-solving skills
- Practice logical reasoning by reimplementing fundamental operations

## Current Scope

### Functions implemented manually (by the author)
- `my_strlen` - string length calculation with buffer overflow protection
- `my_toupper` - uppercase conversion (heap-allocated)
- `my_tolower` - lowercase conversion (heap-allocated)
- `my_strcmp` - string comparison
- `my_strcmp_percent` - similarity via Levenshtein distance
- `min` - minimum of three integers

### UI (AI-generated)
- Raylib-based graphical interface demonstrating each function
- Dark theme with sidebar navigation
- Interactive input fields with TAB to switch between them

## Technical Specifications

| Item | Value |
|------|-------|
| Language | C99 |
| Build | CMake 3.10+ |
| GUI | Raylib |
| Window | 960x680 (fixed) |
| FPS | 60 |
| Max buffer | 256 characters (STR_BUFFER) |
| License | MIT (Lucas Burati, 2026) |

## Documentation

- README in 3 languages: English, Portuguese (BR) and Simplified Chinese
- PKM notes with known bugs, technical debt and future work
