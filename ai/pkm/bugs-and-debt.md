# Known Bugs and Technical Debt

## Bugs - HIGH Severity

### 1. Unsafe atoi() usage
- **File:** `src/main.c:66`
- **Problem:** atoi() has undefined behavior on overflow and cannot distinguish "0" from non-numeric input like "hello"
- **Fix:** Use `strtol()` with error checking

## Bugs - MEDIUM Severity

### 2. Missing const-correctness
- Input parameters should be `const string` to prevent accidental modification
- Prevents safe API usage
- Current build warning: UI passes `const char *` to APIs typed as `string` (`char *`)

### 3. Per-frame allocation in tolower/toupper
- **File:** `src/main.c:39-48`
- At 60 FPS, allocates and frees 256-byte strings 60x/second = unnecessary heap churn

### 4. UI behavior not fully aligned with my_strcmp_percent error codes
- **File:** `src/main.c`, `src/ui.c`
- The backend now returns negative error codes (`-1` to `-4`), but the UI still special-cases empty inputs and does not display every backend error path directly.

### 5. Non-static global Font
- **File:** `src/ui.c`
- `Font mainFont` is globally visible, should be static

## Technical Debt - LOW Severity

### 6. `typedef char * string` obscures pointer semantics
- Pedagogical decision by the author, but may cause confusion about ownership and mutability

### 7. Code duplication between toupper/tolower
- 40+ identical lines between the two functions; could share a helper function

### 8. Hardcoded font path
- `resources/font.ttf` with no fallback if the file doesn't exist

### 9. No automated test suite
- Only manual testing via GUI

### 10. Unused Section enum
- **File:** `include/ui.h`
- `SECTION_STRING, SECTION_MATH` declared but the sidebar uses hardcoded arrays instead

## Recently Fixed / No Longer Current

- `my_strcmp` now stops at the first `\0` and returns the unsigned-character difference, closer to standard `strcmp` behavior.
- `my_strcmp_percent` now checks outer and inner `malloc` calls and frees previously allocated rows on partial allocation failure.
- `my_strlen` now returns `STR_BUFFER + 1` when the buffer limit is reached, separating overflow from an empty string result.
- `string_utils.h` includes `<stddef.h>` for `size_t`.

## Planned

1. Replace `atoi()` with validated parsing.
2. Improve const-correctness in the string utility API.
3. Add automated tests for the backend functions.
4. Align the UI with all `my_strcmp_percent` error codes.
