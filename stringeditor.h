#ifndef STRINGEDITOR_H_INCLUDED
#define STRINGEDITOR_H_INCLUDED

#include <stdio.h>
#include <string.h>

char* creallocStr(char* str);
char** creallocArrayStr(char** str, int str_length);
char* getStr(FILE* input);
char* cutStr(char* str, int start, int endd);
void eraseStr(int start, int endd, char** str);
char** endlSearchStr(char* str, int* k);
int findStr(char* hay, char* needle, int start);
void insertStr(char* needle, char** hay, int start);

char* creallocStr(char* str)
{
    int str_length = strlen(str);
    char* new_str;
    if ((str_length & (str_length + 1))) return str;
    new_str = calloc(sizeof(char), (str_length + 1) * 2);
    memcpy(new_str, str, str_length * sizeof(char));
    free(str);
    return new_str;
}

char** creallocArrayStr(char** str, int str_length)
{
    char** new_str;
    int i;
    if (str_length & (str_length + 1)) return str;
    new_str = calloc(sizeof(char*), (str_length + 1) * 2);
    memcpy(new_str, str, str_length * sizeof(char*));
    free(str);
    return new_str;
}

char* getStr(FILE* input)
{
    char ch = '\0';
    char* str = (char*) calloc(sizeof(char), 1);
    int length = 0;

    if (feof(input))
    {
        free(str);
        return NULL;
    }

    while ((ch = getc(input)) != '\n')
    {
        if (feof(input))
        {
            free(str);
            return NULL;
        }
        str = creallocStr(str);
        str[length] = ch;
        ++length;
    }
    return str;
}

char* cutStr(char* str, int start, int endd)
{
    int i;
    char* result = (char*) calloc (sizeof(char), 1);
    for (i = start; i <= endd; ++i)
    {
        result = creallocStr(result);
        result[i - start] = str[i];
    }
    return result;
}

void eraseStr(int start, int endd, char** str)
{
    char* new_str = (char*) calloc(sizeof(char), (strlen((*str)) - (endd - start + 1)) + 1);
    int i;
    int k = 0;
    for (i = 0; i < start; ++i)
    {
        new_str[k] = (*str)[i];
        ++k;
    }
    for (i = endd + 1; i < strlen((*str)); ++i)
    {
        new_str[k] = (*str)[i];
        ++k;

    }
    free((*str));
    (*str) = new_str;
}

char** endlSearchStr(char* str, int* k)
{
   int len = strlen(str);
   int i;
   char** needles = calloc(sizeof(char*), 1);
   int l = 0;
   (*k) = 0;
   needles[(*k)] = calloc(sizeof(char), 1);
   for (i = 0; i < len; ++i)
   {
        if (i < len - 1 && str[i] == '\\' && str[i + 1] == 'n')
        {
            ++(*k);
            needles = creallocArrayStr(needles, (*k));
            needles[(*k)] = calloc(sizeof(char), 1);
            ++i;
            l = 0;
        }
        else
        {
            needles[(*k)] = creallocStr(needles[(*k)]);
            needles[(*k)][l] = str[i];
            ++l;
        }
    }
    ++(*k);
    return needles;
}

int findStr(char* hay, char* needle, int start)
{
    int i;
    int l_hay = strlen(hay);
    int l_needle = strlen(needle);
    for (i = start; i < l_hay - l_needle + 1; ++i)
    {
        char* str = cutStr(hay, i, i + l_needle - 1);
        if (!strcmp(needle, str))
        {
            free(str);
            return i;
        }
        free(str);
    }
    return -1;
}

void insertStr(char* needle, char** hay, int start)
{
    char* new_str = calloc(sizeof(char*), strlen((*hay)) + strlen(needle) + 1);
    int i;
    for (i = 0; i < start; ++i)
        new_str[i] = (*hay)[i];
    for (i = start; i < start + strlen(needle); ++i)
        new_str[i] = needle[i - start];
    for (i = start + strlen(needle); i < strlen((*hay)) + strlen(needle); ++i)
        new_str[i] = (*hay)[i - strlen(needle)];
    free((*hay));
    (*hay) = new_str;
}

#endif /* STRINGEDITOR_H_INCLUDED */
