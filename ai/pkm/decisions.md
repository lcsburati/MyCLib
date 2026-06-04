# Design Decisions

## 1. Reimplement stdlib functions instead of using existing ones
- **Reason:** Educational purpose - understand how things work under the hood
- **Trade-off:** Functions may have bugs/inefficiencies vs battle-tested libc functions

## 2. `typedef char * string`
- **Reason:** Pedagogical simplification to make code more readable for beginners
- **Trade-off:** Hides pointer semantics, may cause confusion about memory ownership

## 3. Heap allocation for toupper/tolower
- **Reason:** Return a new string without modifying the original (immutability)
- **Trade-off:** Caller must remember to call free(); risk of memory leaks

## 4. Levenshtein Distance for strcmp_percent
- **Reason:** Robust and well-known algorithm for string similarity
- **Trade-off:** O(n*m) complexity in both space and time; dynamically allocates a matrix

## 5. Raylib for UI (AI-generated)
- **Reason:** Interactive visual demonstration of backend functions
- **Trade-off:** Heavy dependency for an educational project; the UI is AI-generated, not by the author

## 6. Backend/UI separation
- **Reason:** Core logic independent from presentation; allows testing backend in isolation
- **Accepted trade-off:** Correct approach - easier maintenance and future testing

## 7. STR_BUFFER = 256 as a global limit
- **Reason:** Prevent buffer overflow; simple and predictable limit
- **Trade-off:** Limits functionality; larger strings are rejected through explicit sentinel/error handling

## 8. my_strlen sentinel values
- **Reason:** Keep the return type as `size_t` while still signaling simple invalid states
- **Current behavior:** `NULL` returns `0`; reaching the buffer limit returns `STR_BUFFER + 1`
- **Trade-off:** `0` still overlaps with empty string for `NULL`, but overflow is now distinguishable from an empty input

## 9. CMake as build system
- **Reason:** Replaced Makefile for better cross-platform portability
- **Migrated in:** commit e8008f8

## 10. README in 3 languages
- **Reason:** Accessibility; international portfolio
- **Languages:** English (main), Portuguese BR, Simplified Chinese

## 11. Negative error codes in my_strcmp_percent
- **Reason:** Keep valid similarity percentages in the `0` to `100` range while separating backend errors
- **Current behavior:** `-1` allocation failure, `-2` overflow detected through `my_strlen`, `-3` NULL input, `-4` both strings empty
- **Trade-off:** The function still mixes status and numeric result in a `double`; a result struct would be cleaner but more complex for the current learning stage
