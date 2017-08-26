#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "stringeditor.h"
#include "comandparcer.h"

struct winsize sz;

void func(int sig)
{
    ioctl(1, TIOCGWINSZ, &sz);
    signal(SIGWINCH, func);
}

/*{this is comment for check delete braces}*/

struct ListNode;

struct ListNode
{
    char* line;
    struct ListNode* next;
};

struct ListNode** creallocListNode(struct ListNode** arr, int arr_length)
{
    int i;
    struct ListNode** new_arr;
    if (arr_length & (arr_length + 1)) return arr;
    new_arr = calloc(sizeof(struct ListNode*), (arr_length + 1) * 2);
    memcpy(new_arr, arr, arr_length * sizeof(struct ListNode*));
    free(arr);
    return new_arr;
}

struct ListNode* printPages(int num_row, int num_col, struct ListNode* list_node)
{
    int line_sz;
    int sum = 0;
    while (list_node)
    {
        if (strlen(list_node->line) % num_col == 0 && strlen(list_node->line) != 0)
            line_sz = strlen(list_node->line) / num_col;
        else
            line_sz = strlen(list_node->line) / num_col + 1;
        if (line_sz > num_row - sum) return list_node;
        printf(">%s\n", list_node->line);
        sum += line_sz;
        list_node = list_node->next;
    }
    return list_node;
}

void printToFile(struct ListNode* fisrt_list_node, FILE* output)
{
    if (!fisrt_list_node) return;
    fprintf(output, ">%s\n", fisrt_list_node->line);
    printToFile(fisrt_list_node->next, output);
}

void printRange(int start, int end, struct ListNode* first_list_node)
{
    int cur = 0;
    struct ListNode* cur_list_node = first_list_node;
    while ((cur <= end || end == -1) && cur_list_node)
    {
        if (cur >= start)
            printf(">%s\n", cur_list_node->line);
        cur_list_node = cur_list_node->next;
        ++cur;
    }
}

void deleteRange(int start, int end, struct ListNode* first_list_node)
{
    int cur = 0;
    struct ListNode* cur_list_node = first_list_node;
    while (cur < start - 1)
    {
        cur_list_node = cur_list_node->next;
        ++cur;
    }
    if (end == -1)
    {
        freeList(cur_list_node->next);
        cur_list_node->next = NULL;
    }
    else
        {
            struct ListNode* cur_list_node_2 = cur_list_node;
            struct ListNode* cur_list_node_3;
            while (cur < end + 1)
            {
                cur_list_node_3 = cur_list_node_2;
                cur_list_node_2 = cur_list_node_2->next;
                ++cur;

                if (cur > start)
                {
                    free(cur_list_node_3->line);
                    free(cur_list_node_3);
                }
            }
            cur_list_node->next = cur_list_node_2;
        }
}

void insertAfterNode(struct ListNode* cur_list_node, struct ListNode* after_insert_list_node, char** insert_str, int n_insert_str, struct ListNode** end_list_node)
{
    int i;
    for (i = 0; i < n_insert_str; ++i)
    {
        struct ListNode* new_list_node = (struct ListNode*) malloc (sizeof(struct ListNode));
        new_list_node->line = insert_str[i];
        new_list_node->next = after_insert_list_node;
        cur_list_node->next = new_list_node;
        cur_list_node = new_list_node;
        (*end_list_node) = cur_list_node;
    }
}

void insertAfter(int dest, char** insert_str, struct ListNode* first_list_node, int n_insert_str)
{
    int cur = 0;
    int i;
    struct ListNode* cur_list_node = first_list_node;
    struct ListNode* end_list_node;
    while ((dest != -1 && cur != dest) || (dest == -1 && cur_list_node->next))
    {
        cur_list_node = cur_list_node->next;
        ++cur;
    }
    insertAfterNode(cur_list_node, cur_list_node->next, insert_str, n_insert_str, &end_list_node);
}

void editString(int num_str, int pos, char insert_char, struct ListNode* first_list_node)
{
    int cur = 0;
    int i;
    struct ListNode* cur_list_node = first_list_node;
    while (cur != num_str)
    {
        ++cur;
        cur_list_node = cur_list_node->next;
    }
    cur_list_node->line[pos] = insert_char;
    if (insert_char == '"' || insert_char == '\\')
    {
        cur_list_node->line = creallocStr(cur_list_node->line);
        for (i = strlen(cur_list_node->line); i > pos; i--)
            cur_list_node->line[i] = cur_list_node->line[i - 1];
        cur_list_node->line[pos] = '\\';
    }
}

void insertSymbol(int num_str, int pos, char insert_char, struct ListNode* first_list_node)
{
    int cur = 0;
    int i;
    struct ListNode* cur_list_node = first_list_node;
    while (cur != num_str)
    {
        ++cur;
        cur_list_node = cur_list_node->next;
    }
    cur_list_node->line = creallocStr(cur_list_node->line);
    for (i = strlen(cur_list_node->line); i > pos; i--)
        cur_list_node->line[i] = cur_list_node->line[i - 1];
    cur_list_node->line[pos] = insert_char;
}

void freeList(struct ListNode* first_list_node)
{
    if (first_list_node == NULL) return;
    freeList(first_list_node->next);
    if(first_list_node->line != NULL)
        free(first_list_node->line);
    free(first_list_node);
}

struct ListNode* read2(char* comand)
{
    char* file_name = cutStr(comand, 5, strlen(comand));
    struct ListNode* first_list_node = (struct ListNode*) malloc (sizeof(struct ListNode));
    struct ListNode* last_list_node;
    FILE* input;
    char* str;

    first_list_node->line = NULL;
    first_list_node->next = NULL;
    last_list_node = first_list_node;

    input = fopen(file_name, "r");

    if (!input)
    {
        printf("No file %s\n", file_name);
        free(file_name);
        return first_list_node;
    }
    while (str = getStr(input))
    {
        struct ListNode* list_node = (struct ListNode*) malloc (sizeof(struct ListNode));
        list_node->line = str;
        list_node->next = NULL;
        last_list_node->next = list_node;
        last_list_node = list_node;
    }
    free(file_name);
    fclose(input);
    return first_list_node;
}

void deleteBracesNode(int* braces, struct ListNode* list_node)
{
    int i;
    for (i = 0; i < strlen(list_node->line); ++i)
    {
        if ((list_node->line)[i] == '{')
        {
            (*braces)++;
            eraseStr(i, i, &(list_node->line));
            i--;
            continue;
        }
        if ((list_node->line)[i] == '}')
        {
            if (braces == 0)
            {
                printf("There is some fail in braces\n");
                return;
            }
            else
            {
                (*braces)--;
                eraseStr(i, i, &(list_node->line));
                i--;
            }
            continue;
        }
        if ((*braces) > 0)
        {
            eraseStr(i, i, &(list_node->line));
            i--;
        }
    }
}

void deleteBraces(int start, int end, struct ListNode* first_list_node)
{
    struct ListNode* cur_list_node = first_list_node->next;
    int cur = 1;
    int braces = 0;
    while ((cur <= end || end == -1) && cur_list_node)
    {
        if (cur >= start) deleteBracesNode(&braces, cur_list_node);
        cur_list_node = cur_list_node->next;
        ++cur;
    }
}

void replaceNode(struct ListNode* list_node, char* hay, char* needle, int start);

void replaceStr(struct ListNode* list_node, char* hay, char* needle, int pos)
{
    int k;
    int i;
    char** needles = (char**) endlSearchStr(needle, &k);
    char** trash = (char**) calloc(sizeof(char*), k - 1);
    char* buf;
    struct ListNode* end_list_node;
    if (k == 1)
    {
        eraseStr(pos, pos + strlen(hay) - 1, &(list_node->line));
        insertStr(needle, &(list_node->line), pos);
        replaceNode(list_node, hay, needle, pos + strlen(needle));

        for (i = 0; i < k; ++i)
            free(needles[i]);
        free(needles);
        free(trash);
        return;
    }
    for (i = 0; i < k - 2; ++i) {
        trash[i] = needles[i + 1];
    }
    buf = cutStr(list_node->line, pos + strlen(hay), strlen(list_node->line) - 1);
    insertStr(needles[k - 1], &buf, 0);
    trash[k - 2] = buf;
    eraseStr(pos, strlen(list_node->line) - 1, &(list_node->line));
    insertStr(needles[0], &(list_node->line), pos);
    insertAfterNode(list_node, list_node->next, trash, k - 1, &end_list_node);
    replaceNode(end_list_node, hay, needle, strlen(needles[k - 1]));
    free(needles[k - 1]);
    free(needles[0]);
    free(needles);
    free(trash);
}

void replaceStrSpecial(struct ListNode* list_node, char* hay, char* needle, int pos)
{
    int k;
    int i;
    struct ListNode* end_list_node;
    char* buf;
    char** needles = endlSearchStr(needle, &k);
    char** trash = calloc(sizeof(char*), k - 1);
    if (k == 1)
    {
        insertStr(needle, &(list_node->line), pos);

        for (i = 0; i < k; ++i)
            free(needles[i]);
        free(needles);
        free(trash);
        return;
    }
    for (i = 0; i < k - 2; ++i) {
        trash[i] = needles[i + 1];
    }
    buf = cutStr(list_node->line, pos, strlen(list_node->line) - 1);
    insertStr(needles[k - 1], &buf, 0);
    trash[k - 2] = buf;
    eraseStr(pos, strlen(list_node->line) - 1, &(list_node->line));
    insertStr(needles[0], &(list_node->line), pos);
    insertAfterNode(list_node, list_node->next, trash, k - 1, &end_list_node);
    free(needles[k - 1]);
    free(needles[0]);
    free(needles);
    free(trash);
}

void replaceNode(struct ListNode* list_node, char* hay, char* needle, int start)
{
    int pos = findStr(list_node->line, hay, start);
    if (!strcmp(hay, "'^'"))
    {
        pos = 0;
        replaceStrSpecial(list_node, hay, needle, pos);
        return;
    }
    if (!strcmp(hay, "'$'"))
    {
        pos = strlen(list_node->line);
        replaceStrSpecial(list_node, hay, needle, pos);
        return;
    }
    if (pos == -1) return;
    replaceStr(list_node, hay, needle, pos);
}


void replace(int start, int end, char* hay, char* needle, struct ListNode* first_list_node)
{
    int i;
    struct ListNode** to_replace = calloc(sizeof(struct ListNode*), 1);
    int cur = 0;
    struct ListNode* cur_list_node = first_list_node;
    int n_to_replace = 0;

    if (start == -1) start = 1;
    while ((end == -1 && cur_list_node) || cur <= end)
    {
        if (cur >= start) {
            to_replace = creallocListNode(to_replace, n_to_replace);
            to_replace[n_to_replace] = cur_list_node;
            ++n_to_replace;
        }
        ++cur;
        cur_list_node = cur_list_node->next;
    }
    for (i = 0; i < n_to_replace; ++i)
    {
        replaceNode(to_replace[i], hay, needle, 0);
    }
    free(to_replace);
}

int checkTerminal()
{

    struct stat sb;
    if (fstat(1, &sb) == -1)
    {
        fprintf(stderr, "I can't fstat\n");
    }
    return S_ISCHR(sb.st_mode);
}

int main(int argc, char** argv)
{
    struct ListNode* first_list_node = (struct ListNode*) malloc (sizeof(struct ListNode));
    struct ListNode* last_list_node;
    FILE* input;
    char* file_to_write = NULL;
    char* comand;
    char* str;
    int i;
    int fl = 0;
    char c;

    first_list_node->line = '\0';
    first_list_node->next = NULL;
    last_list_node = first_list_node;

    if (argc > 1)
    {
        input = fopen(argv[1], "r");
        while (str = getStr(input))
        {
            struct ListNode* list_node = (struct ListNode*) malloc (sizeof(struct ListNode));
            list_node->line = str;
            list_node->next = NULL;
            last_list_node->next = list_node;
            last_list_node = list_node;
        }
        fclose(input);
    }

    ioctl(1, TIOCGWINSZ, &sz);
    signal(SIGWINCH, func);

    /*editor*/
    printf("editor:\n");
    /*get comands*/

    while (1)
    {
        comand_id real_comand;
        comand = getStr(stdin);
        if (comand == NULL)
        {
            freeList(first_list_node);
            free(comand);
            free(file_to_write);
            return 0;
        }
        clean(&comand);
        real_comand = getComand(comand);
        /*print pages*/
        if (real_comand == PRINT_PAGES)
        {
            if (!checkTerminal())
            {
                printToFile(first_list_node->next, stdout);
                free(comand);
                printf("editor:\n");
                continue;
            }
            else
            {
                struct ListNode* current_node = first_list_node->next;
                current_node = printPages(sz.ws_row - 2, sz.ws_col, current_node);

                while (current_node)
                {
                    c = getchar();
                    if (c == ' ')
                        current_node = printPages(sz.ws_row - 2, sz.ws_col, current_node);
                    else
                        if (c == 'q')
                            break;
                        else
                        {
                            printf("wrong comand: try space or q(to exit from print pages)\n");
                            continue;
                        }
                    if (!current_node) break;
                    c = getchar();
                }
                free(comand);
                printf("editor:\n");
            }
        }
        /*print range*/
        if (real_comand == PRINT_RANGE)
        {
            int start, end;
            getRangeParametrs("print range", comand, &start, &end);
            printRange(start, end, first_list_node);
            free(comand);
            printf("editor:\n");
        }
        /*insert after*/
        if (real_comand == INSERT_AFTER)
        {
            int dest = 0;
            char** insert_str = (char**) calloc (sizeof(char*), 1);
            int n_insert_str;
            getInsertAfterParametrs(comand, &dest, &insert_str, &n_insert_str);
            insertAfter(dest, insert_str, first_list_node, n_insert_str);
            free(comand);
            free(insert_str);
            printf("editor:\n");
        }
        /*edit string*/
        if (real_comand == EDIT_STRING)
        {
            int num_str;
            int pos;
            char insert_char;
            getEditStringParametrs("edit string", comand, &num_str, &pos, &insert_char);
            editString(num_str, pos, insert_char, first_list_node);
            free(comand);
            printf("editor:\n");
        }
        /*insert symbol*/
        if (real_comand == INSERT_SYMBOL)
        {
            int num_str;
            int pos;
            char insert_char;
            getInsertSymbolParametrs("insert symbol", comand, &num_str, &pos, &insert_char);
            insertSymbol(num_str, pos, insert_char, first_list_node);
            free(comand);
            printf("editor:\n");
        }
        /*replace*/
        if (real_comand == REPLACE)
        {
            int start, end;
            char* hay;
            char* needle;
            getReplaceParametrs(comand, &start, &end, &hay, &needle);
            replace(start, end, hay, needle, first_list_node);
            free(comand);
            free(hay);
            free(needle);
            printf("editor:\n");
        }
        /*delete range*/
        if (real_comand == DELETE_RANGE)
        {
            int start, end;
            getRangeParametrs("delete range", comand, &start, &end);
            deleteRange(start, end, first_list_node);
            free(comand);
            printf("editor:\n");
        }
        /*delete braces*/
        if (real_comand == DELETE_BRACES)
        {
            int start, end;
            getRangeParametrs("delete braces", comand, &start, &end);
            deleteBraces(start, end, first_list_node);
            free(comand);
            printf("editor:\n");
        }
        /*read*/
        if (real_comand == READ)
        {
            freeList(first_list_node);
            first_list_node = read2(comand);
            free(comand);
            printf("editor:\n");
        }
        /*open*/
        if (real_comand == OPEN){
            freeList(first_list_node);
            file_to_write = cutStr(comand, 5, strlen(comand));
            first_list_node = read2(comand);
            free(comand);
            printf("editor:\n");
        }
        /*write*/
        if (real_comand == WRITE)
        {
            FILE* to_write;
            if (strlen(comand) > 6)
            {
                char* to_write_name = cutStr(comand, 6, strlen(comand));
                to_write = fopen(to_write_name, "w");
                printToFile(first_list_node->next, to_write);
                fclose(to_write);
            }
            else
            {
                if (file_to_write)
                {
                    to_write = fopen(file_to_write, "w");
                    printToFile(first_list_node->next, to_write);
                    fclose(to_write);
                }
                else
                {
                    printf("There are no files to write\n");
                }
            }
            free(comand);
            printf("editor:\n");
        }
        /*help*/
        if (real_comand == HELP)
        {
            struct ListNode* help = read2("read help.txt");
            struct ListNode* current_node = help->next;
            current_node = printPages(sz.ws_row - 2, sz.ws_col, current_node);

            while (current_node)
            {
                c = getchar();
                if (c == ' ')
                    current_node = printPages(sz.ws_row - 2, sz.ws_col, current_node);
                else
                    if (c == 'q')
                        break;
                    else
                    {
                        printf("wrong comand: try space or q(to exit from help)\n");
                        continue;
                    }
                if (!current_node) break;
                c = getchar();
            }
            freeList(help);
            free(comand);
            printf("editor:\n");
        }
        /*exit*/
        if (real_comand == EXIT)
        {
            freeList(first_list_node);
            free(comand);
            free(file_to_write);
            return 0;
        }
    }
    return 0;
}
