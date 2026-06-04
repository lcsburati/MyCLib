# Architecture

## Directory Structure

```
MyCLab/
├── include/                    # Headers
│   ├── string_utils.h          # String function prototypes
│   ├── math_utils.h            # Math function prototypes
│   └── ui.h                    # Raylib UI layer (AI-generated)
│
├── src/                        # Source code
│   ├── string_utils.c          # String implementation (manual)
│   ├── math_utils.c            # Math implementation (manual)
│   ├── main.c                  # Entry point (AI-generated)
│   └── ui.c                    # UI layer (AI-generated)
│
├── resources/
│   └── font.ttf                # Font for rendering
│
├── assets/
│   ├── my_strlen_exemple.png   # Screenshots for documentation
│   └── my_strlen_exemple2.png
│
├── audits/
│   └── code_audit_report.txt   # Code audit report
│
├── ai/
│   ├── agents/auditor.md       # Auditor agent specification
│   └── pkm/                    # This PKM
│
├── CMakeLists.txt
├── README.md / README.pt-br.md / README.zh-cn.md
└── LICENSE
```

## Dependency Graph

```
main.c (entry point)
  ├── ui.h / ui.c ──── raylib.h
  │     Renders sidebar, inputs, pages for each function
  │
  ├── string_utils.h / string_utils.c
  │     Backend logic for strings
  │     Uses: stdlib.h (malloc/free), stdio.h (printf error)
  │     Depends on: math_utils.h (min() for Levenshtein)
  │
  └── math_utils.h / math_utils.c
        Three-value min() function
        Used by: string_utils.c (my_strcmp_percent)
        Used by: main.c (SCREEN_MIN)
```

## Design Principles

1. **Separation of concerns** - Backend (string_utils, math_utils) is independent from the UI
2. **Memory ownership** - my_toupper/my_tolower return heap-allocated strings; the caller must free()
3. **Buffer protection** - Max size validation (STR_BUFFER = 256) in core functions
4. **Error handling** - Uses simple sentinel values: NULL for failed string allocations/conversions, `STR_BUFFER + 1` for strlen overflow, and negative codes for `my_strcmp_percent` errors

## Main Flow

1. `main()` initializes UI via `InitUI()`
2. Main loop:
   - Captures text input in 3 fields (i1, i2, i3)
   - TAB switches active field
   - Switch on current screen calls the corresponding render function
   - Render calls backend functions and displays result
3. `CloseUI()` cleans up resources

## Global State

- `Font mainFont` in ui.c (non-static, coupling risk)
- Inputs i1, i2, i3 in main.c (256-char buffers)
- `currentScreen` in main.c (Screen enum)
