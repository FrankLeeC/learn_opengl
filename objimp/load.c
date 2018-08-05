#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "load.h"
#include "model.h"
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
 * 如果以 \ 结尾，返回true
 */ 
int is_continued(char* s, int len)
{
    return s[len-1] == '\\';
}

typedef struct LineNode
{
    char* line;  // 如果 !is_continued 以 \0 结尾 否则以 \ 结尾
    int is_continued; // 1: y   0: n
    struct LineNode* prev;
    struct LineNode* next;
} LN;

void free_node(LN* node)
{
    free(node->line);
    free(node);
}

void free_node_list(LN* head)
{
    while (head)
    {
        LN* next = head->next;
        free(head);
        head = next;
    }
}

// remove \r
char* remove_return(char* s)
{
    int count = 0;
    int len = strlen(s);
    for (int i=0; i < len; i++)
    {
        if (s[i] == '\r')
        {
            count++;
        }
    }
    if (count)
    {
        char* new_s = malloc((len - count + 1)*sizeof(char));
        int c = 0;
        for (int i = 0; i < len; i++)
        {
            if ('\r' != s[i])
            {
                new_s[c++] = s[i];
            }
        }
        new_s[c] = '\0';
        free(s);
        return new_s;
    }
    return s;
}

char* remove_new_line(char* s)
{
    int len = strlen(s);
    if ('\n' == s[len-1])
    {
        s[len-1] = '\0';
    }
    return s;
}

/**
 * 读取文件 返回链表
 */ 
LN* read_file(char* filename)
{
    FILE* fp = fopen(filename, "r");
    if (!fp) 
        return NULL;
    LN* head = NULL;
    LN* p = NULL;
    while (1)    
    {
        char* line = NULL;
        size_t len;
        int read = getline(&line, &len, fp);
        if (read < 0)
            break;
        line = remove_return(line);
        line = remove_new_line(line);
        if (!is_comment_or_empty(line))
        {
            LN* n = (LN*)malloc(sizeof(LN));
            if (!n)
            {
                p = head;
                while (p)
                {
                    LN* tmp = p->next;
                    free(p);
                    p = tmp;
                }
                return NULL;
            }
            if (is_continued(line, strlen(line)))
            {
                line[strlen(line)] = '\0';
                n->line = line;
                n->is_continued = 1;
            }
            else 
            {
                line[strlen(line)] = '\0';
                n->line = line;
                n->is_continued = 0;
            }

            if (head == NULL)
            {
                head = n;
                p = head;
            }
            else
            {
                p->next = n;
                n->prev = p;
                p = n;
            }
            
        }
        
    }
    return head;
}

/**
 * 根据结尾符 '\' 合并行
 * 成功返回 1 否则 0
 */ 
int merge(LN* head)
{
    LN* node = head;
    while (node) 
    {
        while (node->is_continued)
        {
            int n_len = strlen(node->line);
            if (node->next)
            {
                LN* next = node->next;
                int nn_len = strlen(next->line);
                node->is_continued = next->is_continued;
                char* tmp = (char*)realloc(node->line, (n_len+nn_len)*sizeof(char));
                if (!tmp)
                    return 0;
                if (tmp != node->line)
                {
                    free(node->line);
                    node->line = tmp;
                }
                
                for (int i = 0; i < nn_len; i++)
                {
                    node->line[n_len-1+i] = next->line[i];
                }
                node->line[n_len+nn_len-1] = '\0';
                node->next = next->next;
                if (next->next)
                {
                    next->next->prev = node;
                }
                free_node(next);
            }
           
            else
            {
                node->line[n_len-1] = '\0';
            }
        }
        node = node->next;
    }
    return 1;
}

void tranverse(LN* head)
{
    LN* node = head;
    while (node)
    {
        printf("%d---%s\n", strlen(node->line), node->line);
        node = node->next;
    }
}

void test()
{
    LN* head = read_file("./text.txt");
    tranverse(head);
    printf("======================\n");
    merge(head);
    tranverse(head);
}

int main() 
{
    test();
    return 0;
}