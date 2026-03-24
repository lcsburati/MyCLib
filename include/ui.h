#ifndef UI_H
#define UI_H

#include "raylib.h"

#define WIN_W            960
#define WIN_H            680
#define SIDEBAR_W        200
#define CONTENT_X        (SIDEBAR_W + 40)
#define CONTENT_W        (WIN_W - SIDEBAR_W - 64)
#define INPUT_H          48
#define BTN_H            40
#define SECTION_H        36
#define FONT_SIZE_LG     26
#define FONT_SIZE_MD     20
#define FONT_SIZE_SM     16
#define FONT_SIZE_XS     13
#define MAX_INPUT        256

#define BG_DARK          (Color){ 26,  26,  30,  255 }
#define BG_SIDEBAR       (Color){ 30,  30,  36,  255 }
#define PANEL_BG         (Color){ 38,  38,  46,  255 }
#define PANEL_HOVER      (Color){ 50,  50,  60,  255 }
#define INPUT_BG         (Color){ 34,  34,  40,  255 }
#define INPUT_BORDER     (Color){ 60,  60,  72,  255 }
#define INPUT_FOCUS      (Color){ 80,  140, 255,  255 }
#define ACCENT           (Color){ 80,  140, 255,  255 }
#define ACCENT_DIM       (Color){ 50,  100, 200,  255 }
#define GREEN_OK         (Color){ 60,  200, 130,  255 }
#define RED_ERR          (Color){ 220, 80,  90,   255 }
#define ORANGE_WARN      (Color){ 240, 160, 60,   255 }
#define TEXT_PRIMARY     (Color){ 240, 240, 245,  255 }
#define TEXT_SECONDARY   (Color){ 155, 155, 170,  255 }
#define TEXT_MUTED       (Color){ 95,  95,  110,  255 }
#define DIVIDER          (Color){ 48,  48,  56,   255 }
#define SECTION_BG       (Color){ 45,  45,  55,   255 }

typedef enum {
    SCREEN_STRLEN,
    SCREEN_TOLOWER,
    SCREEN_TOUPPER,
    SCREEN_STRCMP,
    SCREEN_STRCMPPCT,
    SCREEN_MIN,
    SCREEN_COUNT
} Screen;

typedef enum {
    SECTION_STRING,
    SECTION_MATH
} Section;

// UI Life Cycle
void InitUI(void);
void CloseUI(void);

// UI Components
int DrawNavButton(Rectangle rec, const char *label, int isActive, float *hoverAnim);
void DrawInputField(Rectangle rec, const char *buffer, const char *placeholder, int isFocused);
void DrawResultCard(const char *label, const char *value, float y, Color accentColor);
void DrawPercentageBar(float y, float width, double percent);
void HandleTextInput(char *buffer, int *count, int maxLen);

// High-level UI Layouts
void DrawSidebar(Screen *currentScreen, char *i1, int *c1, char *i2, int *c2, char *i3, int *c3, int *activeField, float *navHover);

// Screen Rendering
void RenderStrlenPage(const char *input, int *activeField, int len);
void RenderToLowerPage(const char *input, int *activeField, const char *result);
void RenderToUpperPage(const char *input, int *activeField, const char *result);
void RenderStrcmpPage(const char *i1, const char *i2, int *activeField, int cmpResult);
void RenderStrcmpPctPage(const char *i1, const char *i2, int *activeField, double pct, int dist);
void RenderMinPage(const char *i1, const char *i2, const char *i3, int *activeField, int result);

#endif
