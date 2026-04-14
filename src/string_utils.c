/*Includes C Standarts Libs*/
#include <stdio.h>
#include <stdlib.h>

/*Includes My C Libs*/
#include "math_utils.h" 
#include "string_utils.h"

/*Functions*/
size_t my_strlen(string str)
{
    if (str == NULL)
    {
        return 1;
    }

    size_t length = 0;

    while (*str && length < STR_BUFFER)
    {
        length++, str++;
    }

    return length;
}

string my_toupper(string str)
{
    if (str == NULL)
    {
        return NULL;
    }

    size_t length = my_strlen(str);
    string result = malloc(sizeof(char) * (length + 1));
    if (result == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    string current = result;

    while (*str && length < STR_BUFFER)
    {
        if (*str >= 'a' && *str <= 'z')
        {
            *current = *str - OFFSET;
            str++;
            current++;
        }
        else
        {
            *current = *str;
            str++;
            current++;
        }
    }
    *current = '\0';
    return result;
}

string my_tolower(string str)
{
    if (str == NULL)
    {
        return NULL;
    }

    size_t length = my_strlen(str);
    string result = malloc(sizeof(char) * (length + 1));
    if (result == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    string current = result;


    while (*str && length < STR_BUFFER)
    {
        if (*str >= 'A' && *str <= 'Z')
        {
            *current = *str + OFFSET;
            str++;
            current++;
        } 
        else
        {
            *current = *str;
            str++;
            current++;
        }
    }
    *current = '\0';


    return result;
}

int my_strcmp(string str1, string str2)
{
    if (str1 == NULL || str2 == NULL)
    {
        return 1;
    }

    size_t str1_length = my_strlen(str1);
    size_t str2_length = my_strlen(str2);

    if (str1_length > str2_length && str1_length < STR_BUFFER && str2_length < STR_BUFFER)
    {
        while (*str1)
        {
            if (*str1 != *str2)
            {
                return 1;
            }
            str1++; str2++;
        }
    }
    else if (str1_length < str2_length && str1_length < STR_BUFFER && str2_length < STR_BUFFER)
    {
        while (*str2)
        {
            if (*str2 != *str1)
            {
                return -1;
            }
            str1++; str2++;
        }
    }

    else if (str1_length == str2_length && str1_length < STR_BUFFER && str2_length < STR_BUFFER)
    {
        while (*str1)
        {
            if (*str1 != *str2)
            {
                return 1;
            }
            str1++; str2++;
        }

    }
    
    return 0;


}

double my_strcmp_percent(string str1, string str2)
{
    if (str1 == NULL || str2 == NULL)
    {
        return 1;
    }

    /*Variables*/
    size_t str1_length = my_strlen(str1);
    size_t str2_length = my_strlen(str2);
    
    /*Heap Space Allocation*/
    int **matrix = malloc((str1_length + 1) * sizeof(int*));
    for (int i = 0; i <= str1_length; i++) {
        matrix[i] = malloc((str2_length + 1) * sizeof(int));
    }

    /*Border Definitions*/
    for (int i = 0; i <= str1_length; i++) {
        matrix[i][0] = i;
    }
    for (int j = 0; j <= str2_length; j++) {
        matrix[0][j] = j;
    }

    /*Operations*/
    for (int i = 1; i <= str1_length; i++) {
        for (int j = 1; j <= str2_length; j++) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            matrix[i][j] = min(
                matrix[i - 1][j] + 1,       
                matrix[i][j - 1] + 1,       
                matrix[i - 1][j - 1] + cost 
            );
        }
    }

    int distance = matrix[str1_length][str2_length];
    int max_len = (str1_length > str2_length) ? str1_length : str2_length;
    
    if (max_len == 0) return 100.0;
    
    double percent = 100.0 * (1.0 - (double)distance / max_len);

    for (int i = 0; i <= str1_length; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return percent;
}