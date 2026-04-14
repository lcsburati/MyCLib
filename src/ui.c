#include <stdlib.h>

#include "ui.h"
#include "string_utils.h"

Font mainFont;

// Sidebar navigation labels and icon info (now internal to UI)
static const char *navLabels[SCREEN_COUNT] = {
    "strlen", "tolower", "toupper", "strcmp", "strcmp %", "min"
};
static const char *sectionLabels[] = { "STRING UTILS", "MATH UTILS" };
static int sectionStarts[] = { 0, 5 };

void InitUI(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WIN_W, WIN_H, "MyCLib - C Libraries");
    SetTargetFPS(60);
    mainFont = LoadFontEx("resources/font.ttf", 96, 0, 0);
    SetTextureFilter(mainFont.texture, TEXTURE_FILTER_BILINEAR);
}

void CloseUI(void) {
    UnloadFont(mainFont);
    CloseWindow();
}

int DrawNavButton(Rectangle rec, const char *label, int isActive, float *hoverAnim) {
    Vector2 mouse = GetMousePosition();
    int over = CheckCollisionPointRec(mouse, rec);
    int clicked = over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    float target = (over || isActive) ? 1.0f : 0.0f;
    *hoverAnim += (target - *hoverAnim) * 0.15f;

    Color bg = isActive ? ACCENT_DIM : (over ? PANEL_HOVER : PANEL_BG);
    DrawRectangleRounded(rec, 0.08f, 6, bg);

    if (isActive) {
        DrawRectangleRounded(
            (Rectangle){ rec.x, rec.y + 8, 3, rec.height - 16 },
            1.0f, 4, ACCENT
        );
    }

    Color textCol = isActive ? TEXT_PRIMARY : (over ? TEXT_PRIMARY : TEXT_SECONDARY);
    DrawTextEx(mainFont, label, (Vector2){ (float)(rec.x + 16), (float)(rec.y + (rec.height - 16) / 2) }, 16, 1, textCol);

    return clicked;
}

void DrawInputField(Rectangle rec, const char *buffer, const char *placeholder, int isFocused) {
    DrawRectangleRounded(rec, 0.1f, 6, INPUT_BG);
    Color borderCol = isFocused ? INPUT_FOCUS : INPUT_BORDER;
    DrawRectangleRoundedLines(rec, 0.1f, 6, borderCol);

    float textY = rec.y + (rec.height - FONT_SIZE_SM) / 2;
    if (buffer[0] == '\0' && !isFocused) {
        DrawTextEx(mainFont, placeholder, (Vector2){ (float)(rec.x + 16), (float)textY }, (float)FONT_SIZE_SM, 1, TEXT_MUTED);
    } else {
        DrawTextEx(mainFont, buffer, (Vector2){ (float)(rec.x + 16), (float)textY }, (float)FONT_SIZE_SM, 1, TEXT_PRIMARY);
    }

    if (isFocused && ((int)(GetTime() * 2.5f) % 2 == 0)) {
        Vector2 textW = MeasureTextEx(mainFont, buffer, (float)FONT_SIZE_SM, 1);
        DrawRectangle((int)(rec.x + 16 + textW.x + 2), (int)(rec.y + 14), 2, (int)(rec.height - 28), ACCENT);
    }
}

void DrawResultCard(const char *label, const char *value, float y, Color accentColor) {
    Rectangle card = { CONTENT_X, y, CONTENT_W, 72 };
    DrawRectangleRounded(card, 0.1f, 8, PANEL_BG);
    DrawRectangleRoundedLines(card, 0.1f, 8, INPUT_BORDER);
    DrawRectangleRounded((Rectangle){ card.x, card.y + 12, 3, card.height - 24 }, 1.0f, 4, accentColor);
    DrawTextEx(mainFont, label, (Vector2){ (float)(card.x + 20), (float)(card.y + 14) }, (float)FONT_SIZE_XS, 1, TEXT_SECONDARY);
    DrawTextEx(mainFont, value, (Vector2){ (float)(card.x + 20), (float)(card.y + 36) }, (float)FONT_SIZE_MD, 1, accentColor);
}

void DrawPercentageBar(float y, float width, double percent) {
    Rectangle outer = { CONTENT_X, y, width, 28 };
    DrawRectangleRounded(outer, 0.3f, 6, PANEL_BG);
    Color barColor = (percent >= 80.0) ? GREEN_OK : (percent >= 50.0 ? ORANGE_WARN : RED_ERR);
    float fillW = (width - 4) * (float)(percent / 100.0);
    if (fillW < 4) fillW = 4;
    Rectangle fill = { CONTENT_X + 2, y + 2, fillW, 24 };
    DrawRectangleRounded(fill, 0.3f, 6, barColor);
    const char *pctText = TextFormat("%.1f%%", percent);
    Vector2 tw = MeasureTextEx(mainFont, pctText, (float)FONT_SIZE_XS, 1);
    DrawTextEx(mainFont, pctText, (Vector2){ (float)(CONTENT_X + width / 2 - tw.x / 2), (float)(y + 7) }, (float)FONT_SIZE_XS, 1, TEXT_PRIMARY);
}

void HandleTextInput(char *buffer, int *count, int maxLen) {
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
    if (IsKeyDown(KEY_BACKSPACE) && IsKeyPressedRepeat(KEY_BACKSPACE) && *count > 0) {
        (*count)--;
        buffer[*count] = '\0';
    }
}

void DrawSidebar(Screen *currentScreen, char *i1, int *c1, char *i2, int *c2, char *i3, int *c3, int *activeField, float *navHover) {
    DrawRectangle(0, 0, SIDEBAR_W, WIN_H, BG_SIDEBAR);
    DrawRectangle(SIDEBAR_W - 1, 0, 1, WIN_H, DIVIDER);
    DrawTextEx(mainFont, "MyCLib", (Vector2){ 20, 20 }, 22, 1, TEXT_PRIMARY);
    DrawTextEx(mainFont, "C Libraries", (Vector2){ 20, 46 }, 12, 1, TEXT_SECONDARY);
    DrawRectangle(20, 70, SIDEBAR_W - 40, 1, DIVIDER);

    int baseY = 90;
    for (int s = 0; s < 2; s++) {
        DrawTextEx(mainFont, sectionLabels[s], (Vector2){ 20, (float)baseY }, 10, 1, TEXT_MUTED);
        baseY += 18;
        int start = sectionStarts[s];
        int end = (s == 0) ? 5 : SCREEN_COUNT;
        for (int i = start; i < end; i++) {
            Rectangle btn = { 10, (float)baseY, SIDEBAR_W - 20, BTN_H };
            if (DrawNavButton(btn, navLabels[i], *currentScreen == i, &navHover[i])) {
                if (*currentScreen != i) {
                    *currentScreen = (Screen)i;
                    i1[0] = '\0'; *c1 = 0;
                    i2[0] = '\0'; *c2 = 0;
                    i3[0] = '\0'; *c3 = 0;
                    *activeField = 1;
                }
            }
            baseY += BTN_H + 4;
        }
        baseY += 12;
    }
    DrawRectangle(20, WIN_H - 50, SIDEBAR_W - 40, 1, DIVIDER);
    DrawTextEx(mainFont, "C99 + raylib", (Vector2){ 20, (float)(WIN_H - 36) }, 11, 1, TEXT_MUTED);
}

static void PageHeader(const char *title, const char *subtitle, Color accent) {
    DrawTextEx(mainFont, title, (Vector2){ (float)CONTENT_X, 24 }, FONT_SIZE_LG, 1, TEXT_PRIMARY);
    DrawTextEx(mainFont, subtitle, (Vector2){ (float)CONTENT_X, 60 }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    DrawRectangle(CONTENT_X, 82, 60, 3, accent);
}

void RenderStrlenPage(const char *input, int *activeField, int len) {
    PageHeader("my_strlen", "Calculate the length of a string", ACCENT);
    DrawTextEx(mainFont, "Input", (Vector2){ (float)CONTENT_X, 100 }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    Rectangle field = { CONTENT_X, 118, CONTENT_W, INPUT_H };
    DrawInputField(field, input, "Type something...", *activeField == 1);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), field)) *activeField = 1;
    
    if (input[0] == '\0') {
        DrawResultCard("Length", "Empty input (0)", 200, ORANGE_WARN);
    } else {
        DrawResultCard("Length", TextFormat("%d characters", len), 200, ACCENT);
    }
    
    float barY = 300;
    DrawTextEx(mainFont, "Character count", (Vector2){ (float)CONTENT_X, barY }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    barY += 20;
    Rectangle barOuter = { CONTENT_X, barY, CONTENT_W, 20 };
    DrawRectangleRounded(barOuter, 0.4f, 6, PANEL_BG);
    float fillPct = (float)len / (STR_BUFFER - 1);
    if (fillPct > 1.0f) fillPct = 1.0f;
    DrawRectangleRounded((Rectangle){ CONTENT_X + 2, barY + 2, (CONTENT_W - 4) * fillPct, 16 }, 0.4f, 6, ACCENT);
    DrawTextEx(mainFont, TextFormat("%d / %d", len, STR_BUFFER - 1), (Vector2){ (float)CONTENT_X, barY + 28 }, FONT_SIZE_XS, 1, TEXT_MUTED);
}

void RenderToLowerPage(const char *input, int *activeField, const char *result) {
    PageHeader("my_tolower", "Convert text to lowercase", GREEN_OK);
    DrawTextEx(mainFont, "Input", (Vector2){ (float)CONTENT_X, 100 }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    Rectangle field = { CONTENT_X, 118, CONTENT_W, INPUT_H };
    DrawInputField(field, input, "TYPE SOMETHING HERE...", *activeField == 1);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), field)) *activeField = 1;
    if (result[0] == '\0' && input[0] != '\0') {
        DrawResultCard("Result", "NULL input detected", 200, RED_ERR);
    } else {
        DrawResultCard("Result", result[0] != '\0' ? result : "(empty)", 200, GREEN_OK);
    }
}

void RenderToUpperPage(const char *input, int *activeField, const char *result) {
    PageHeader("my_toupper", "Convert text to UPPERCASE", ACCENT);
    DrawTextEx(mainFont, "Input", (Vector2){ (float)CONTENT_X, 100 }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    Rectangle field = { CONTENT_X, 118, CONTENT_W, INPUT_H };
    DrawInputField(field, input, "type something here...", *activeField == 1);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), field)) *activeField = 1;
    if (result[0] == '\0' && input[0] != '\0') {
        DrawResultCard("Result", "NULL input detected", 200, RED_ERR);
    } else {
        DrawResultCard("Result", result[0] != '\0' ? result : "(empty)", 200, ACCENT);
    }
}

void RenderStrcmpPage(const char *i1, const char *i2, int *activeField, int cmpResult) {
    PageHeader("my_strcmp", "Compare two strings lexicographically", ACCENT);
    DrawTextEx(mainFont, "String 1", (Vector2){ (float)CONTENT_X, 100 }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    Rectangle f1 = { CONTENT_X, 118, CONTENT_W, INPUT_H };
    DrawInputField(f1, i1, "First string...", *activeField == 1);
    DrawTextEx(mainFont, "String 2", (Vector2){ (float)CONTENT_X, 188 }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    Rectangle f2 = { CONTENT_X, 206, CONTENT_W, INPUT_H };
    DrawInputField(f2, i2, "Second string...", *activeField == 2);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), f1)) *activeField = 1;
        if (CheckCollisionPointRec(GetMousePosition(), f2)) *activeField = 2;
    }

    int len1 = my_strlen(i1);
    int len2 = my_strlen(i2);
    int empty1 = (i1[0] == '\0');
    int empty2 = (i2[0] == '\0');
    int bothEmpty = empty1 && empty2;
    int oneEmpty = empty1 != empty2;
    int sameLen = (len1 == len2);

    if (bothEmpty) {
        DrawTextEx(mainFont, "Length: S1=0 | S2=0 | SAME SIZE", (Vector2){ (float)CONTENT_X, 280 }, FONT_SIZE_XS, 1, ORANGE_WARN);
        DrawResultCard("Comparison Result", "EMPTY INPUTS - EQUAL (0)", 308, GREEN_OK);
    } else if (oneEmpty) {
        const char *sizeLabel = empty1 ? "S1 EMPTY" : "S2 EMPTY";
        DrawTextEx(mainFont, TextFormat("Length: S1=%d | S2=%d | %s", len1, len2, sizeLabel), (Vector2){ (float)CONTENT_X, 280 }, FONT_SIZE_XS, 1, RED_ERR);
        DrawResultCard("Comparison Result", empty1 ? "S2 GREATER (1)" : "S1 GREATER (1)", 308, RED_ERR);
    } else {
        const char *sizeLabel = sameLen ? "SAME SIZE" : "DIFFERENT SIZES";
        DrawTextEx(mainFont, TextFormat("Length: S1=%d | S2=%d | %s", len1, len2, sizeLabel), (Vector2){ (float)CONTENT_X, 280 }, FONT_SIZE_XS, 1, sameLen ? ORANGE_WARN : TEXT_MUTED);

        const char *msg;
        Color col;
        if (cmpResult == 0) {
            msg = "EQUAL (0)";
            col = GREEN_OK;
        } else if (cmpResult > 0) {
            msg = sameLen ? "DIFFERENT - S1 > S2 (1)" : "S1 GREATER LENGTH (1)";
            col = RED_ERR;
        } else {
            msg = "S1 < S2 (-1)";
            col = ORANGE_WARN;
        }
        DrawResultCard("Comparison Result", msg, 308, col);
    }
}

void RenderStrcmpPctPage(const char *i1, const char *i2, int *activeField, double pct, int dist) {
    PageHeader("my_strcmp_percent", "Similarity using Levenshtein distance", ACCENT);
    DrawTextEx(mainFont, "String 1", (Vector2){ (float)CONTENT_X, 100 }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    Rectangle f1 = { CONTENT_X, 118, CONTENT_W, INPUT_H };
    DrawInputField(f1, i1, "First string...", *activeField == 1);
    DrawTextEx(mainFont, "String 2", (Vector2){ (float)CONTENT_X, 188 }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    Rectangle f2 = { CONTENT_X, 206, CONTENT_W, INPUT_H };
    DrawInputField(f2, i2, "Second string...", *activeField == 2);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), f1)) *activeField = 1;
        if (CheckCollisionPointRec(GetMousePosition(), f2)) *activeField = 2;
    }

    int empty1 = (i1[0] == '\0');
    int empty2 = (i2[0] == '\0');
    int bothEmpty = empty1 && empty2;

    if (bothEmpty) {
        DrawResultCard("Similarity", "Empty inputs (100%)", 288, ORANGE_WARN);
        DrawPercentageBar(388, CONTENT_W, 100.0);
        DrawTextEx(mainFont, "Edit distance: 0 operations", (Vector2){ (float)CONTENT_X, 434 }, FONT_SIZE_XS, 1, TEXT_MUTED);
    } else {
        DrawResultCard("Similarity", TextFormat("%.1f%% match", pct), 288, (pct >= 80.0 ? GREEN_OK : (pct >= 50.0 ? ORANGE_WARN : RED_ERR)));
        DrawPercentageBar(388, CONTENT_W, pct);
        DrawTextEx(mainFont, TextFormat("Edit distance: %d operations", dist), (Vector2){ (float)CONTENT_X, 434 }, FONT_SIZE_XS, 1, TEXT_MUTED);
    }
}

void RenderMinPage(const char *i1, const char *i2, const char *i3, int *activeField, int result) {
    PageHeader("my_min", "Returns the minimum of three integers", ORANGE_WARN);
    DrawTextEx(mainFont, "Enter three integers", (Vector2){ (float)CONTENT_X, 100 }, FONT_SIZE_XS, 1, TEXT_SECONDARY);
    int fW = (CONTENT_W - 24) / 3;
    Rectangle f1 = { CONTENT_X, 118, (float)fW, INPUT_H };
    Rectangle f2 = { CONTENT_X + fW + 12, 118, (float)fW, INPUT_H };
    Rectangle f3 = { CONTENT_X + 2 * (fW + 12), 118, (float)fW, INPUT_H };
    DrawInputField(f1, i1, "a", *activeField == 1);
    DrawInputField(f2, i2, "b", *activeField == 2);
    DrawInputField(f3, i3, "c", *activeField == 3);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), f1)) *activeField = 1;
        if (CheckCollisionPointRec(GetMousePosition(), f2)) *activeField = 2;
        if (CheckCollisionPointRec(GetMousePosition(), f3)) *activeField = 3;
    }
    DrawResultCard("Minimum", TextFormat("%d", result), 200, ORANGE_WARN);
    DrawTextEx(mainFont, "Formula: min(a, b, c) - returns the smallest of three values", (Vector2){ (float)CONTENT_X, 300 }, FONT_SIZE_XS, 1, TEXT_MUTED);
}
