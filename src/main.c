// ═══════════════════════════════════════════════════════════════
//  STRING UTILS ENGINE — Modern Visual Interface
//  Built with raylib · All custom string functions
// ═══════════════════════════════════════════════════════════════
#include "raylib.h"
#include "string_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ─────────────────────────────────────────────
//  COLOR PALETTE — Modern dark theme
// ─────────────────────────────────────────────
#define BG_DARK         (Color){ 12,  12,  16,  255 }
#define BG_SIDEBAR      (Color){ 18,  18,  24,  255 }
#define PANEL_BG        (Color){ 22,  22,  30,  255 }
#define PANEL_SURFACE   (Color){ 30,  30,  42,  255 }
#define PANEL_HOVER     (Color){ 38,  38,  52,  255 }
#define INPUT_BG        (Color){ 16,  16,  22,  255 }
#define INPUT_BORDER    (Color){ 50,  50,  68,  255 }
#define ACCENT          (Color){ 99,  102, 241, 255 }   // Indigo
#define ACCENT_BRIGHT   (Color){ 129, 140, 248, 255 }
#define ACCENT_DIM      (Color){ 55,  48,  163, 255 }
#define ACCENT_GLOW     (Color){ 99,  102, 241, 40  }
#define GREEN_OK        (Color){ 34,  197, 94,  255 }
#define GREEN_DIM       (Color){ 22,  101, 52,  255 }
#define RED_ERR         (Color){ 239, 68,  68,  255 }
#define ORANGE_WARN     (Color){ 251, 146, 60,  255 }
#define TEXT_PRIMARY     (Color){ 240, 240, 245, 255 }
#define TEXT_SECONDARY  (Color){ 148, 148, 168, 255 }
#define TEXT_MUTED      (Color){ 82,  82,  100, 255 }
#define BORDER_SUBTLE   (Color){ 40,  40,  55,  255 }
#define DIVIDER         (Color){ 35,  35,  48,  255 }

// ─────────────────────────────────────────────
//  LAYOUT CONSTANTS
// ─────────────────────────────────────────────
#define WIN_W           960
#define WIN_H           640
#define SIDEBAR_W       220
#define CONTENT_X       (SIDEBAR_W + 32)
#define CONTENT_W       (WIN_W - SIDEBAR_W - 64)
#define INPUT_H         48
#define BTN_H           44
#define FONT_SIZE_LG    28
#define FONT_SIZE_MD    20
#define FONT_SIZE_SM    16
#define FONT_SIZE_XS    13
#define MAX_INPUT       128

// ─────────────────────────────────────────────
//  SCREENS
// ─────────────────────────────────────────────
typedef enum {
    SCREEN_STRLEN,
    SCREEN_TOLOWER,
    SCREEN_TOUPPER,
    SCREEN_STRCMP,
    SCREEN_STRCMPPCT,
    SCREEN_COUNT
} Screen;

// ColorLerp is provided by raylib

// ─────────────────────────────────────────────
//  DRAW: Sidebar nav button
// ─────────────────────────────────────────────
static int DrawNavButton(Rectangle rec, const char *label, const char *icon,
                         int isActive, float *hoverAnim) {
    Vector2 mouse = GetMousePosition();
    int over = CheckCollisionPointRec(mouse, rec);
    int clicked = over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    // Animate hover
    float target = (over || isActive) ? 1.0f : 0.0f;
    *hoverAnim += (target - *hoverAnim) * 8.0f * GetFrameTime();

    Color bg = ColorLerp(BG_SIDEBAR, PANEL_SURFACE, *hoverAnim);
    if (isActive) bg = ACCENT_DIM;

    DrawRectangleRounded(rec, 0.3f, 6, bg);

    if (isActive) {
        // Active indicator bar
        DrawRectangleRounded(
            (Rectangle){ rec.x, rec.y + 8, 3, rec.height - 16 },
            1.0f, 4, ACCENT_BRIGHT
        );
    }

    // Icon
    DrawText(icon, (int)(rec.x + 14), (int)(rec.y + (rec.height - 16) / 2), 16,
             isActive ? ACCENT_BRIGHT : TEXT_SECONDARY);

    // Label
    Color textCol = isActive ? TEXT_PRIMARY : TEXT_SECONDARY;
    textCol = over ? TEXT_PRIMARY : textCol;
    DrawText(label, (int)(rec.x + 38), (int)(rec.y + (rec.height - 16) / 2), 16, textCol);

    return clicked;
}

// ─────────────────────────────────────────────
//  DRAW: Text input field
// ─────────────────────────────────────────────
static void DrawInputField(Rectangle rec, const char *buffer, const char *placeholder,
                           int isFocused) {
    // Background
    DrawRectangleRounded(rec, 0.15f, 6, INPUT_BG);
    // Border
    Color borderCol = isFocused ? ACCENT : INPUT_BORDER;
    DrawRectangleRoundedLines(rec, 0.15f, 6, borderCol);

    // Glow when focused
    if (isFocused) {
        DrawRectangleRounded(
            (Rectangle){ rec.x - 2, rec.y - 2, rec.width + 4, rec.height + 4 },
            0.15f, 6, ACCENT_GLOW
        );
        DrawRectangleRounded(rec, 0.15f, 6, INPUT_BG);
        DrawRectangleRoundedLines(rec, 0.15f, 6, ACCENT);
    }

    float textY = rec.y + (rec.height - FONT_SIZE_SM) / 2;

    if (buffer[0] == '\0' && !isFocused) {
        // Placeholder
        DrawText(placeholder, (int)(rec.x + 16), (int)textY, FONT_SIZE_SM, TEXT_MUTED);
    } else {
        DrawText(buffer, (int)(rec.x + 16), (int)textY, FONT_SIZE_SM, TEXT_PRIMARY);
    }

    // Blinking cursor
    if (isFocused && ((int)(GetTime() * 2.5f) % 2 == 0)) {
        int textW = MeasureText(buffer, FONT_SIZE_SM);
        DrawRectangle((int)(rec.x + 16 + textW + 2),
                      (int)(rec.y + 12), 2, (int)(rec.height - 24), ACCENT);
    }
}

// ─────────────────────────────────────────────
//  DRAW: Section header with accent line
// ─────────────────────────────────────────────
static void DrawSectionHeader(const char *title, const char *subtitle, float y) {
    DrawText(title, CONTENT_X, (int)y, FONT_SIZE_LG, TEXT_PRIMARY);
    if (subtitle) {
        DrawText(subtitle, CONTENT_X, (int)(y + FONT_SIZE_LG + 6), FONT_SIZE_XS, TEXT_SECONDARY);
    }
    DrawRectangle(CONTENT_X, (int)(y + FONT_SIZE_LG + (subtitle ? 28 : 10)), 48, 3, ACCENT);
}

// ─────────────────────────────────────────────
//  DRAW: Result card
// ─────────────────────────────────────────────
static void DrawResultCard(const char *label, const char *value, float y, Color accentColor) {
    Rectangle card = { CONTENT_X, y, CONTENT_W, 72 };

    // Card background
    DrawRectangleRounded(card, 0.12f, 6, PANEL_BG);
    DrawRectangleRoundedLines(card, 0.12f, 6, BORDER_SUBTLE);

    // Left accent strip
    DrawRectangleRounded(
        (Rectangle){ card.x, card.y + 8, 3, card.height - 16 },
        1.0f, 4, accentColor
    );

    // Label
    DrawText(label, (int)(card.x + 20), (int)(card.y + 14), FONT_SIZE_XS, TEXT_SECONDARY);

    // Value
    DrawText(value, (int)(card.x + 20), (int)(card.y + 36), FONT_SIZE_MD, accentColor);
}

// ─────────────────────────────────────────────
//  DRAW: Progress / percentage bar
// ─────────────────────────────────────────────
static void DrawPercentageBar(float y, double percent) {
    Rectangle outer = { CONTENT_X, y, CONTENT_W, 28 };
    DrawRectangleRounded(outer, 0.5f, 6, PANEL_BG);

    Color barColor;
    if (percent >= 80.0) barColor = GREEN_OK;
    else if (percent >= 50.0) barColor = ORANGE_WARN;
    else barColor = RED_ERR;

    float fillW = (float)(CONTENT_W - 4) * (float)(percent / 100.0);
    if (fillW < 4) fillW = 4;
    Rectangle fill = { CONTENT_X + 2, y + 2, fillW, 24 };
    DrawRectangleRounded(fill, 0.5f, 6, barColor);

    // Percentage text on bar
    const char *pctText = TextFormat("%.1f%%", percent);
    int tw = MeasureText(pctText, FONT_SIZE_XS);
    float textX = CONTENT_X + CONTENT_W / 2 - tw / 2;
    DrawText(pctText, (int)textX, (int)(y + 7), FONT_SIZE_XS, TEXT_PRIMARY);
}

// ─────────────────────────────────────────────
//  HANDLE TEXT INPUT
// ─────────────────────────────────────────────
static void HandleTextInput(char *buffer, int *count, int maxLen) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (*count < maxLen - 1)) {
            buffer[*count] = (char)key;
            (*count)++;
            buffer[*count] = '\0';
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && *count > 0) {
        (*count)--;
        buffer[*count] = '\0';
    }
    // Hold backspace for rapid delete
    if (IsKeyDown(KEY_BACKSPACE) && IsKeyPressedRepeat(KEY_BACKSPACE) && *count > 0) {
        (*count)--;
        buffer[*count] = '\0';
    }
}

// ═════════════════════════════════════════════
//  MAIN
// ═════════════════════════════════════════════
int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WIN_W, WIN_H, "String Utils Engine");
    SetTargetFPS(60);

    Screen currentScreen = SCREEN_STRLEN;

    char input1[MAX_INPUT] = "";
    char input2[MAX_INPUT] = "";
    int count1 = 0;
    int count2 = 0;
    int activeField = 1;

    float navHover[SCREEN_COUNT] = {0};

    const char *navLabels[SCREEN_COUNT] = {
        "strlen",
        "tolower",
        "toupper",
        "strcmp",
        "strcmp %",
    };
    const char *navIcons[SCREEN_COUNT] = {
        "#", "a", "A", "=", "%",
    };

    while (!WindowShouldClose()) {

        // ── INPUT ─────────────────────────────
        if (activeField == 1) {
            HandleTextInput(input1, &count1, MAX_INPUT);
        } else {
            HandleTextInput(input2, &count2, MAX_INPUT);
        }

        // TAB switches fields
        if (IsKeyPressed(KEY_TAB)) {
            activeField = (activeField == 1) ? 2 : 1;
        }

        // ── DRAW ──────────────────────────────
        BeginDrawing();
        ClearBackground(BG_DARK);

        // ── SIDEBAR ───────────────────────────
        DrawRectangle(0, 0, SIDEBAR_W, WIN_H, BG_SIDEBAR);
        DrawRectangle(SIDEBAR_W - 1, 0, 1, WIN_H, BORDER_SUBTLE);

        // Logo area
        DrawText("STRING", 20, 24, 22, TEXT_PRIMARY);
        DrawText("UTILS", 20, 48, 22, ACCENT);
        DrawRectangle(20, 80, SIDEBAR_W - 40, 1, DIVIDER);

        DrawText("TOOLS", 20, 96, FONT_SIZE_XS, TEXT_MUTED);

        // Nav buttons
        for (int i = 0; i < SCREEN_COUNT; i++) {
            Rectangle btn = { 10, 118 + i * (BTN_H + 6), SIDEBAR_W - 20, BTN_H };
            if (DrawNavButton(btn, navLabels[i], navIcons[i],
                              currentScreen == i, &navHover[i])) {
                if (currentScreen != i) {
                    currentScreen = i;
                    // Reset inputs on screen change
                    input1[0] = '\0'; count1 = 0;
                    input2[0] = '\0'; count2 = 0;
                    activeField = 1;
                }
            }
        }

        // Sidebar footer
        DrawRectangle(20, WIN_H - 50, SIDEBAR_W - 40, 1, DIVIDER);
        DrawText("v2.0  |  C99 + raylib", 20, WIN_H - 36, 11, TEXT_MUTED);

        // ── CONTENT AREA ──────────────────────
        switch (currentScreen) {

            // ══════════════════════════════════
            case SCREEN_STRLEN: {
                DrawSectionHeader("my_strlen", "Calculate the length of a string", 28);

                DrawText("Input", CONTENT_X, 90, FONT_SIZE_XS, TEXT_SECONDARY);
                Rectangle field = { CONTENT_X, 110, CONTENT_W, INPUT_H };
                DrawInputField(field, input1, "Type something...", activeField == 1);

                // Click to focus
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                    CheckCollisionPointRec(GetMousePosition(), field)) {
                    activeField = 1;
                }

                // Result
                int len = my_strlen(input1);
                DrawResultCard("Length", TextFormat("%d characters", len), 190, ACCENT_BRIGHT);

                // Visual bar representation
                float barY = 290;
                DrawText("Visual representation", CONTENT_X, (int)barY, FONT_SIZE_XS, TEXT_SECONDARY);
                barY += 22;

                Rectangle barOuter = { CONTENT_X, barY, CONTENT_W, 20 };
                DrawRectangleRounded(barOuter, 0.5f, 6, PANEL_BG);

                float maxChars = (float)(MAX_INPUT - 1);
                float fillPct = (float)len / maxChars;
                if (fillPct > 1.0f) fillPct = 1.0f;
                float fillW = (CONTENT_W - 4) * fillPct;
                if (fillW < 2) fillW = 2;
                Rectangle barFill = { CONTENT_X + 2, barY + 2, fillW, 16 };
                DrawRectangleRounded(barFill, 0.5f, 6, ACCENT);

                DrawText(TextFormat("%d / %d", len, MAX_INPUT - 1),
                         CONTENT_X, (int)(barY + 28), FONT_SIZE_XS, TEXT_MUTED);

                break;
            }

            // ══════════════════════════════════
            case SCREEN_TOLOWER: {
                DrawSectionHeader("my_tolower", "Convert text to lowercase", 28);

                DrawText("Input", CONTENT_X, 90, FONT_SIZE_XS, TEXT_SECONDARY);
                Rectangle field = { CONTENT_X, 110, CONTENT_W, INPUT_H };
                DrawInputField(field, input1, "TYPE SOMETHING HERE...", activeField == 1);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                    CheckCollisionPointRec(GetMousePosition(), field)) {
                    activeField = 1;
                }

                char *result = my_tolower(input1);
                const char *display = (result != NULL) ? result : "";
                DrawResultCard("Result", display, 190, GREEN_OK);
                if (result) free(result);

                break;
            }

            // ══════════════════════════════════
            case SCREEN_TOUPPER: {
                DrawSectionHeader("my_toupper", "Convert text to UPPERCASE", 28);

                DrawText("Input", CONTENT_X, 90, FONT_SIZE_XS, TEXT_SECONDARY);
                Rectangle field = { CONTENT_X, 110, CONTENT_W, INPUT_H };
                DrawInputField(field, input1, "type something here...", activeField == 1);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                    CheckCollisionPointRec(GetMousePosition(), field)) {
                    activeField = 1;
                }

                char *result = my_toupper(input1);
                const char *display = (result != NULL) ? result : "";
                DrawResultCard("Result", display, 190, GREEN_OK);
                if (result) free(result);

                break;
            }

            // ══════════════════════════════════
            case SCREEN_STRCMP: {
                DrawSectionHeader("my_strcmp", "Compare two strings", 28);

                // Field 1
                DrawText("String 1", CONTENT_X, 90, FONT_SIZE_XS, TEXT_SECONDARY);
                Rectangle field1 = { CONTENT_X, 110, CONTENT_W, INPUT_H };
                DrawInputField(field1, input1, "First string...", activeField == 1);

                // Field 2
                DrawText("String 2", CONTENT_X, 178, FONT_SIZE_XS, TEXT_SECONDARY);
                Rectangle field2 = { CONTENT_X, 198, CONTENT_W, INPUT_H };
                DrawInputField(field2, input2, "Second string...", activeField == 2);

                // Focus handling
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(GetMousePosition(), field1)) activeField = 1;
                    if (CheckCollisionPointRec(GetMousePosition(), field2)) activeField = 2;
                }

                int cmp = my_strcmp(input1, input2);
                const char *cmpText;
                Color resColor;
                if (cmp == 0) {
                    cmpText = "EQUAL  (return: 0)";
                    resColor = GREEN_OK;
                } else if (cmp < 0) {
                    cmpText = "String 1 < String 2";
                    resColor = ORANGE_WARN;
                } else {
                    cmpText = "String 1 > String 2";
                    resColor = RED_ERR;
                }

                DrawResultCard("Comparison", cmpText, 280, resColor);

                break;
            }

            // ══════════════════════════════════
            case SCREEN_STRCMPPCT: {
                DrawSectionHeader("my_strcmp_percent",
                                  "Similarity percentage (Levenshtein distance)", 28);

                // Field 1
                DrawText("String 1", CONTENT_X, 90, FONT_SIZE_XS, TEXT_SECONDARY);
                Rectangle field1 = { CONTENT_X, 110, CONTENT_W, INPUT_H };
                DrawInputField(field1, input1, "First string...", activeField == 1);

                // Field 2
                DrawText("String 2", CONTENT_X, 178, FONT_SIZE_XS, TEXT_SECONDARY);
                Rectangle field2 = { CONTENT_X, 198, CONTENT_W, INPUT_H };
                DrawInputField(field2, input2, "Second string...", activeField == 2);

                // Focus handling
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(GetMousePosition(), field1)) activeField = 1;
                    if (CheckCollisionPointRec(GetMousePosition(), field2)) activeField = 2;
                }

                // Compute similarity
                double pct = 0.0;
                if (input1[0] != '\0' || input2[0] != '\0') {
                    pct = my_strcmp_percent(input1, input2);
                }

                DrawResultCard("Similarity",
                               TextFormat("%.1f%% match", pct), 280,
                               pct >= 80.0 ? GREEN_OK : (pct >= 50.0 ? ORANGE_WARN : RED_ERR));

                DrawPercentageBar(372, pct);

                // Levenshtein distance info
                int l1 = my_strlen(input1);
                int l2 = my_strlen(input2);
                int maxL = l1 > l2 ? l1 : l2;
                int dist = (maxL == 0) ? 0 : (int)((1.0 - pct / 100.0) * maxL + 0.5);

                DrawText(TextFormat("Edit distance: %d operations", dist),
                         CONTENT_X, 418, FONT_SIZE_XS, TEXT_MUTED);

                break;
            }

            default: break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}