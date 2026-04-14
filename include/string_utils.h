/*Header Guard*/
#ifndef STRING_UTILS_H
#define STRING_UTILS_H


/*Constants*/
#define OFFSET ('a' - 'A')
#define STR_BUFFER 256

/*Structs*/
typedef char * string;

/*Prototypes*/
size_t my_strlen(string str);
string my_toupper(string str);
string my_tolower(string str);
int my_strcmp(string str1, string str2);
double my_strcmp_percent(string str1, string str2);

/*Closing Header Guard Condition*/
#endif