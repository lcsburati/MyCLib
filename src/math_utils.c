#include "math_utils.h"

int min(int a, int b, int c)
{
    int temp = (a < b) ? a : b;
    return (temp < c) ? temp : c;
}