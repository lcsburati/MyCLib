# AGENTS.md — MyCLab

Guidance for AI coding agents working in this repository. Read this before making changes.

## What this project is

**MyCLab** is an *educational* C99 project that re-implements C standard-library
functions (`string.h`, parts of `stdlib.h`) from scratch, to understand how the
language works under the hood. A Raylib GUI demonstrates each function interactively.

The goal is **learning**, not production-grade libc. Correctness, clarity, and the
author's own reasoning matter more than micro-optimizations or "clever" rewrites.

## ⚠️ Authorship boundary — the most important rule

This repo has a deliberate split between hand-written and AI-generated code. Respect it.

| Area | Author | Agent behavior |
|------|--------|----------------|
| `src/string_utils.c`, `src/math_utils.c`, `include/string_utils.h`, `include/math_utils.h` | **Human, no AI** | This is the author's *learning* code. Do **not** rewrite, "optimize", or replace logic unless explicitly asked. Suggest and explain; let the author write. |
| `src/main.c`, `src/ui.c`, `include/ui.h` | **AI-generated** | The Raylib UI layer. Safe to edit freely when asked. |

When in doubt about whether a change belongs to learning code, ask first. Never silently
"improve" the manual implementations — that defeats the project's purpose. If you produce
any code that lands in the repo, it must be clearly attributable as AI-generated (e.g. in
the commit message / report), to preserve the integrity stated in the README's
"Important Notice".

## ⚠️ Do not touch version control

Leave **git** to the author. Agents must **not** run git operations or change git config:
no `git add` / `commit` / `push` / `checkout` / `reset`, no editing `.gitignore`, and no
"just checking" commands such as `git status` / `git check-ignore` — unless the author
explicitly asks. Make your file changes and stop; the author decides what gets tracked
and committed.

## Build, run, and check

Requires **CMake ≥ 3.10**, a **C99 compiler** (Clang/GCC), and **Raylib** installed on the system.

```bash
mkdir -p build && cd build
cmake ..
make
./main          # launches the 960x680 Raylib window
```

Compiler flags (from `CMakeLists.txt`): `-Wall -D_DEFAULT_SOURCE -Wno-missing-braces`.

For a senior-style review build, prefer high-warning flags and sanitizers (the project has
no automated test suite yet — verification is manual via the GUI):

```bash
clang -std=c99 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion \
      -fsanitize=address,undefined -O0 -g3 src/string_utils.c src/math_utils.c <your-test-driver>.c
```

Resources: the build copies `resources/` (contains `font.ttf`, required at runtime) into
the build dir automatically. Linking is platform-specific (macOS frameworks / Windows
opengl32,gdi32,winmm / Linux raylib,m,pthread,dl) — see `CMakeLists.txt`.

## Layout

```
include/        # headers (string_utils.h, math_utils.h, ui.h)
src/            # implementations (string_utils.c, math_utils.c = manual; main.c, ui.c = AI)
resources/      # font.ttf (copied into build/)
assets/         # screenshots for README
audits/         # generated code-audit / action reports (plain text)
ai/agents/      # agent specifications (e.g. auditor.md)
ai/pkm/         # Personal Knowledge Management — deep project docs (see below)
build/, obj/    # build artifacts (gitignored)
README*.md      # docs in English, pt-BR, zh-CN
```

## Dependency direction

```
main.c ──► ui.h/ui.c ──► raylib
       └─► string_utils ──► math_utils (min() for Levenshtein)
                        └─► stdlib.h (malloc/free), stdio.h (printf on error)
```

Backend (`string_utils`, `math_utils`) is independent of the UI — keep it that way.

## Conventions in the manual code

- `typedef char * string;` — a pedagogical alias for `char*`. Intentional, even though
  hiding pointers behind typedefs is normally discouraged. Don't "fix" it without asking.
- `#define OFFSET ('a' - 'A')` (= 32) and `#define STR_BUFFER 256` — `#define` over `const`
  is a deliberate choice for these.
- **Memory ownership:** `my_toupper` / `my_tolower` return a **heap-allocated** string;
  the *caller must `free()`*. They return `NULL` on invalid/empty input.
- **Error sentinels:** `my_strlen` returns `0` on `NULL` or on reaching `STR_BUFFER`
  (note: this conflates "empty" with "error/overflow" — a known limitation).
- Style: Allman braces, 4-space indent, comment headers grouping sections.

## Public API (quick reference)

| Function | Header | Returns |
|----------|--------|---------|
| `size_t my_strlen(string)` | string_utils.h | length, or 0 on NULL/overflow |
| `string my_toupper(string)` | string_utils.h | new heap string (free it) / NULL |
| `string my_tolower(string)` | string_utils.h | new heap string (free it) / NULL |
| `int my_strcmp(string, string)` | string_utils.h | 0 if equal; difference of first mismatching byte; nonzero sentinel if either arg is NULL |
| `double my_strcmp_percent(string, string)` | string_utils.h | 0.0–100.0 similarity (Levenshtein) |
| `int min(int, int, int)` | math_utils.h | smallest of three |

Full reference with line numbers: `ai/pkm/api.md`.

## Known issues & technical debt

Do **not** rediscover these from scratch — they are catalogued in
[`ai/pkm/bugs-and-debt.md`](ai/pkm/bugs-and-debt.md). Highlights: `my_strcmp` semantics
differ from standard `strcmp` (and historically over-read past `\0`); unchecked `malloc`
in `my_strcmp_percent`; `atoi()` in `main.c` (use `strtol`); signed/unsigned warnings;
missing `<stddef.h>` for `size_t`; per-frame allocations in the UI loop.

## Where to get context (read the PKM first)

`ai/pkm/` is the canonical knowledge base — consult it before large changes:

- `overview.md` — purpose, scope, specs
- `architecture.md` — structure, dependency graph, global state
- `api.md` — every public function with file:line
- `build.md` — build system & platform linking
- `decisions.md` — design trade-offs (why things are the way they are)
- `bugs-and-debt.md` — known bugs / TODOs
- `history.md` — milestones

If you make a structural change, update the relevant PKM file to keep it in sync.

## Audit workflow

`ai/agents/auditor.md` defines a read-only senior-C-auditor agent. When running an audit:
write a single plain-text report into `audits/`, **do not modify source**, exclude the
AI-generated UI files (`ui.h`, `ui.c`, `main.c`) and the dirs below, and always state in
the report that the UI layer was AI-generated.

## Ignore / don't touch

Skip these for analysis: `build/`, `obj/`, `*.o`, `*.plist` (clang-analyzer output),
`.DS_Store`, and the agent dirs `.opencode/` and `.claude/`. What is tracked or committed
is the author's call — see *Do not touch version control* above.
