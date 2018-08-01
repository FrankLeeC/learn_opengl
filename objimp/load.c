#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load.h"
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
    LN* prev;
    LN* next;
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
        int len;
        int read = getline(&line, &len, fp);
        if (read < 0)
            break;
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
            if (is_continued(line, read))
            {
                n->line = line;
                n->is_continued = 1;
            }
            else 
            {
                line[read] = '\0';
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
                node->line[n_len-1] = ' ';
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
                    node->line[n_len+i] = next->line[i];
                }
                
                node->next = next->next;
                next->next->prev = node;
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

int main() 
{
    char* s = "";
    printf("%d\n", is_comment_or_empty(s));
    s = NULL;
    printf("%d\n", is_comment_or_empty(s));
    return 0;
}