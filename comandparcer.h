#ifndef COMANDPARCER_H_INCLUDED
#define COMANDPARCER_H_INCLUDED

#define comand_id int

#define PRINT_PAGES 1
#define PRINT_RANGE 2
#define INSERT_AFTER 3
#define EDIT_STRING 4
#define REPLACE 5
#define DELETE_RANGE 6
#define DELETE_BRACES 7
#define READ 8
#define OPEN 9
#define WRITE 10
#define HELP 11
#define EXIT 12
#define INSERT_SYMBOL 13

comand_id getComand(char *comand);
void getRangeParametrs(char* real_comand, char* comand, int* start, int* end);
void getInsertAfterParametrs(char* comand, int* dest, char*** insert_str, int* n_insert_str);
void getReplaceParametrs(char* comand, int* start, int* end, char** hay, char** needle);

comand_id getComand(char *comand)
{
    char* trash = (char*) calloc(sizeof(char), 1);
    int i = 0;
    while (i < strlen(comand) && comand[i] != ' ')
    {
        trash = creallocStr(trash);
        trash[i] = comand[i];
        ++i;
    }
    if (!strcmp("replace", trash))
    {
        free(trash);
        return REPLACE;
    }
    if (!strcmp("exit", trash))
    {
        free(trash);
        return EXIT;
    }
    if (!strcmp("read", trash))
    {
        free(trash);
        return READ;
    }
    if (!strcmp("open", trash))
    {
        free(trash);
        return OPEN;
    }
    if (!strcmp("help", trash))
    {
        free(trash);
        return HELP;
    }
    if (!strcmp("write", trash))
    {
        free(trash);
        return WRITE;
    }
    trash = creallocStr(trash);
    trash[i] = ' ';
    ++i;
    while (i < strlen(comand) && comand[i] != ' ')
    {
        trash = creallocStr(trash);
        trash[i] = comand[i];
        ++i;
    }
    if (!strcmp("print pages", trash))
    {
        free(trash);
        return PRINT_PAGES;
    }
    if (!strcmp("print range", trash))
    {
        free(trash);
        return PRINT_RANGE;
    }
    if (!strcmp("insert after", trash))
    {
        free(trash);
        return INSERT_AFTER;
    }
    if (!strcmp("edit string", trash))
    {
        free(trash);
        return EDIT_STRING;
    }
    if (!strcmp("delete range", trash))
    {
        free(trash);
        return DELETE_RANGE;
    }
    if (!strcmp("delete braces", trash))
    {
        free(trash);
        return DELETE_BRACES;
    }
    if (!strcmp("insert symbol", trash))
    {
        free(trash);
        return INSERT_SYMBOL;
    }
    printf("Comand %s not found\n", trash);
    free(trash);
    free(comand);
    return -1;
}

void getRangeParametrs(char* real_comand, char* comand, int* start, int* end)
{
    int length = strlen(real_comand);
    int i = length + 1, j = 0;
    char* trash = (char*) calloc(sizeof(char), 1);
    if (i >= strlen(comand))
    {
        (*start) =  -1;
        (*end) = -1;
        free(trash);
        return;
    }
    while (i < strlen(comand) && comand[i] != ' ')
    {
        trash = creallocStr(trash);
        trash[j] = comand[i];
        ++i; ++j;
    }
    (*start) = atoi(trash);
    ++i;
    if (i >= strlen(comand))
    {
        (*end) =  -1;
        free(trash);
        return;
    }
    j = 0;
    free(trash);
    trash = (char*) calloc(sizeof(char), 1);
    while (i < strlen(comand) && comand[i] != ' ')
    {
        trash = creallocStr(trash);
        trash[j] = comand[i];
        ++i; ++j;
    }

    (*end) = atoi(trash);
    free(trash);
}

void getInsertAfterParametrs(char* comand, int* dest, char*** insert_str, int* n_insert_str)
{
    char* trash = (char*) calloc(sizeof(char), 1);
    int i = 13;
    int j;
    (*n_insert_str) = 1;
    if (comand[i] == '"')
    {
        (*dest) = -1;
        i = 12;
    }
    if ((*dest) != -1)
    {
        j = 0;
        while (i < strlen(comand) && comand[i] != ' ')
        {
            trash = creallocStr(trash);
            trash[j] = comand[i];
            ++i; ++j;
        }
        (*dest) = atoi(trash);
    }
    free(trash);
    trash = cutStr(comand, i + 2, strlen(comand) - 2);

    (*insert_str) = creallocArrayStr((*insert_str), (*n_insert_str) + 1);
    (*insert_str)[0] = trash;
    if (strcmp(trash, "\""))
    {
        return;
    }
    i = 0;
    (*n_insert_str) = 0;
    free(trash);
    while(trash = getStr(stdin))
    {
        if (!strcmp(trash, "\"\"\""))
        {
            free(trash);
            return;
        }
        (*insert_str) = creallocArrayStr((*insert_str), (*n_insert_str) + 1);
        (*insert_str)[i] = trash;
        ++(*n_insert_str);
        ++i;
    }
}

void getEditStringParametrs(char* real_comand, char* comand, int* start, int* end, char* insert_char)
{
    int length = strlen(real_comand);
    int i = length + 1, j = 0;
    char* trash = (char*) calloc(sizeof(char), 1);
    while (i < strlen(comand) && comand[i] != ' ')
    {
        trash = creallocStr(trash);
        trash[j] = comand[i];
        ++i; ++j;
    }
    (*start) = atoi(trash);
    ++i;
    j = 0;
    free(trash);
    trash = (char*) calloc(sizeof(char), 1);
    while (i < strlen(comand) && comand[i] != ' ')
    {
        trash = creallocStr(trash);
        trash[j] = comand[i];
        ++i; ++j;
    }
    (*end) = atoi(trash);
    ++i;
    (*insert_char) = comand[i];
    free(trash);
}

void getInsertSymbolParametrs(char* real_comand, char* comand, int* start, int* end, char* insert_char)
{
    getEditStringParametrs(real_comand, comand, start, end, insert_char);
}

void getReplaceParametrs(char* comand, int* start, int* end, char** hay, char** needle)
{
    /*getRange*/
    int i = 8;
    if (i >= strlen(comand) || comand[i] == '\'' || comand[i] == '"')
    {
        (*start) = -1;
        (*end) = -1;
    }
    else
    {
        char* trash = (char*) calloc(sizeof(char), 1);
        int j = 0;
        while (i < strlen(comand) && comand[i] != ' ')
        {
            trash = creallocStr(trash);
            trash[j] = comand[i];
            ++i; ++j;
        }
        (*start) = atoi(trash);
        free(trash);
        ++i;
        if (i >= strlen(comand) || comand[i] == '\'' || comand[i] == '"')
        {
            (*end) = -1;
        }
        else
        {
            char* trash = (char*) calloc(sizeof(char), 1);
            int j = 0;
            while (i < strlen(comand) && comand[i] != ' ')
            {
                trash = creallocStr(trash);
                trash[j] = comand[i];
                ++i; ++j;
            }
            (*end) = atoi(trash);
            free(trash);
            ++i;
        }
    }
    /*end of getRange*/
    /*getHay*/
    if (i >= strlen(comand))
    {
        comand = getStr(stdin);
        i = 0;
    }
    if (comand[i] == '\'')
    {
        char* trash = (char*) calloc(sizeof(char), 1);
        int j = 0;
        while (i < strlen(comand) && comand[i] != ' ')
        {
            trash = creallocStr(trash);
            trash[j] = comand[i];
            ++i; ++j;
        }
        (*hay) = trash;
        ++i;
        ++i;
    }
    else
    {
        char* trash = (char*) calloc(sizeof(char), 1);
        int j = 0;
        ++i;
        while (i < strlen(comand) && comand[i] != '"')
        {
            trash = creallocStr(trash);
            trash[j] = comand[i];
            ++i; ++j;
        }
        (*hay) = trash;
        ++i;
        ++i;
        ++i;
    }
    /*end of getHay*/
    /*getNeedle*/
    if (i >= strlen(comand))
    {
        comand = getStr(stdin);
        i = 1;
    }
    {
        char* trash = (char*) calloc(sizeof(char), 1);
        int j = 0;
        while (i < strlen(comand) && comand[i] != '"')
        {
            trash = creallocStr(trash);
            trash[j] = comand[i];
            ++i; ++j;
        }
        (*needle) = trash;
    }
    /*end of getNeedle*/
}

void clean(char** comand)
{
    int j;
    for (j = 0; j < strlen((*comand)); ++j)
    {
        if ((*comand)[j] == '#')
        {
            eraseStr(j, strlen((*comand)) - 1, comand);
            return;
        }
        if ((*comand)[j] == ' ' && (j == 0 || (*comand)[j - 1] == ' '))
        {
            eraseStr(j, j, comand);
            j--;
        }
    }
}

#endif /* COMANDPARCER_H_INCLUDED */
