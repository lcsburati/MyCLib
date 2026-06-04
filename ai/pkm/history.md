# Project History

## Evolution

The project evolved in distinct phases:

### Phase 1 - Foundation
- Initial implementation of string functions (strlen, toupper, tolower, strcmp)
- Output via terminal/CLI

### Phase 2 - Levenshtein
- Added `my_strcmp_percent` with Levenshtein Distance algorithm
- Commit: `2b2ecd7`

### Phase 3 - Graphical Interface
- Applied Raylib for visual UI
- Introduced `typedef string`
- Commit: `9759aa5`

### Phase 4 - Build System
- Migrated from Makefile to CMake
- UI improvements
- Commit: `e8008f8`

### Phase 5 - Refactoring
- Decoupled UI logic from main.c
- Commit: `e287e0f`
- Renamed MAX_INPUT -> STR_BUFFER
- Moved buffer validation to core functions
- Changed return types to size_t
- Commit: `366b6f5`

### Phase 6 - Documentation and Quality
- READMEs in 3 languages
- Improved string validation
- Commits: `43db9c5`, `920b3bc`
- MIT License, PKM notes, improved buffer handling
- Commit: `74ca5d7` (HEAD)

### Phase 7 - Audit (current)
- Code audit by AI agent
- Identified high severity bugs
- Project PKM created

## Milestones

| Date | Milestone |
|------|-----------|
| - | First my_strlen implementation |
| - | Levenshtein distance implemented |
| - | Raylib GUI added |
| - | Migrated to CMake |
| - | Separation of concerns refactoring |
| 2026-04-22 | Code audit completed |
| 2026-04-22 | PKM created |
