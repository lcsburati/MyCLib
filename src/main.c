#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "string_utils.h"
#include "math_utils.h"
#include "ui.h"

int main(void) {
    InitUI();

    Screen currentScreen = SCREEN_STRLEN;
    char i1[MAX_INPUT] = "", i2[MAX_INPUT] = "", i3[MAX_INPUT] = "";
    int c1 = 0, c2 = 0, c3 = 0, activeField = 1;
    float navHover[SCREEN_COUNT] = {0};

    while (!WindowShouldClose()) {
        // --- Input Handling ---
        if (activeField == 1) HandleTextInput(i1, &c1, MAX_INPUT);
        else if (activeField == 2) HandleTextInput(i2, &c2, MAX_INPUT);
        else HandleTextInput(i3, &c3, MAX_INPUT);

        if (IsKeyPressed(KEY_TAB)) {
            activeField = (activeField % 3) + 1;
        }

        // --- Rendering ---
        BeginDrawing();
        ClearBackground(BG_DARK);

        DrawSidebar(&currentScreen, i1, &c1, i2, &c2, i3, &c3, &activeField, navHover);

        switch (currentScreen) {
            case SCREEN_STRLEN:
                RenderStrlenPage(i1, &activeField, my_strlen(i1));
                break;

            case SCREEN_TOLOWER: {
                char *res = my_tolower(i1);
                RenderToLowerPage(i1, &activeField, res ? res : "");
                if (res) free(res);
                break;
            }

            case SCREEN_TOUPPER: {
                char *res = my_toupper(i1);
                RenderToUpperPage(i1, &activeField, res ? res : "");
                if (res) free(res);
                break;
            }

            case SCREEN_STRCMP:
                RenderStrcmpPage(i1, i2, &activeField, my_strcmp(i1, i2));
                break;

            case SCREEN_STRCMPPCT: {
                double pct = (i1[0] != '\0' || i2[0] != '\0') ? my_strcmp_percent(i1, i2) : 0.0;
                int l1 = my_strlen(i1), l2 = my_strlen(i2);
                int maxL = l1 > l2 ? l1 : l2;
                int dist = (maxL == 0) ? 0 : (int)((1.0 - pct / 100.0) * maxL + 0.5);
                RenderStrcmpPctPage(i1, i2, &activeField, pct, dist);
                break;
            }

            case SCREEN_MIN:
                RenderMinPage(i1, i2, i3, &activeField, min(atoi(i1), atoi(i2), atoi(i3)));
                break;

            default: break;
        }

        EndDrawing();
    }

    CloseUI();
    return 0;
}
