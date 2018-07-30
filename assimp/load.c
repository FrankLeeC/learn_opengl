#include <stdio.h>
#include <stdlib.h>
#include "load.h"
#include <mycommon/strutil.h>


int is_comment_or_empty(char* str) 
{
    if (str == NULL) 
    {
        return 1;
    }
    for (int i = 0; str[i] != '\0'; i++) 
    {
        if (str[i] != '\t' && str[i] != ' ') 
        {
            if (str[i] == '#') 
            {
                return 1;
            }
            return 0;
        }
    }
    return 1;
}

/**
 * read file line by line
 */
int read_file(char* file_name, char*** my_lines, int* count, int** my_length) 
{
    FILE * fp = NULL;
    int n = 2;
    int m = 2;
    char** lines = (char**)malloc(n*sizeof(char*));
    int* length = (int*)malloc(m*sizeof(int));
    size_t len = 0;
    int read;

    fp = fopen(file_name, "r");
    if (fp == NULL) 
    {
        for(int i = 0; i < n; i++) 
        {
            free(lines[i]);
        }
        free(lines);
        lines = NULL;
        free(length);
        length = NULL;
        return -2;
    }
    int i = 0;
    while (1) 
    {
        char * line = NULL;
        if((read = getline(&line, &len, fp)) != -1 && !is_comment_or_empty(line)) 
        {
            line[read] = '\0';
            lines[i] = line;
            length[i] = read;
            i++;
            if (i >= n) 
            {
                char** tmp = (char**)realloc(lines, n*2*sizeof(char*));
                if (tmp) 
                {
                    lines = tmp;
                    n *= 2;
                }
                else 
                {
                    for(int i = 0; i < n; i++) 
                    {
                        free(lines[i]);
                    }
                    free(lines);
                    lines = NULL;
                    return -3;
                }
                tmp = NULL;
            }
            if (i >= m) 
            {
                int* tmp = (int*)realloc(length, m*2*sizeof(int));
                if (tmp) 
                {
                    length = tmp;
                    m *= 2;
                }
                else
                {
                    free(length);
                    length = NULL;
                    return -3;
                }
                tmp = NULL;
            }
        }
        else 
        {
            break;
        }
    }
    *count = i;
    int r = fclose(fp);
    if (r) 
    {
        for(int i = 0; i < n; i++) 
        {
            free(lines[i]);
        }
        free(lines);
        lines = NULL;

        free(length);
        length = NULL;
        return -1;
    }
    *my_lines = lines;
    *my_length = length;
    return 0;
}



int main() 
{
    char* s = "";
    printf("%d\n", is_comment_or_empty(s));
    s = NULL;
    printf("%d\n", is_comment_or_empty(s));
    return 0;
}