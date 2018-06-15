#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

char** read_file(char* file_name, int* count, int** my_length) {
    __FILE * fp = NULL;
    int n = 2;
    int m = 2;
    char** lines = (char**)malloc(n*sizeof(char*));
    int* length = (int*)malloc(m*sizeof(int));
    size_t len = 0;
    int read;

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        for(int i = 0; i < n; i++) {
            free(lines[i]);
        }
        free(lines);
        lines = NULL;

        free(length);
        length = NULL;
        return NULL;
    }
    int i = 0;
    while (1) {
        char * line = NULL;
        if((read = getline(&line, &len, fp)) != -1) {
            // printf("Retrieved line of length %zu/%d :%s", read, len, line);
            line[read] = NULL;
            lines[i] = line;
            length[i] = read;
            i++;
            if (i >= n) {
                char** tmp = (char**)realloc(lines, n*2*sizeof(char*));
                if (tmp) {
                    lines = tmp;
                    n *= 2;
                } else {
                    free(*lines);
                    lines = NULL;
                    return NULL;
                }
                tmp = NULL;
            }
            if (i >= m) {
                int* tmp = (int*)realloc(length, m*2*sizeof(int));
                if (tmp) {
                    length = tmp;
                    m *= 2;
                } else {
                    free(length);
                    length = NULL;
                    return -3;
                }
                tmp = NULL;
            }
        } else {
            break;
        }
    }
    *count = i;
    int r = fclose(fp);
    if (r) {
        for(int i = 0; i < n; i++) {
            free(lines[i]);
        }
        free(lines);
        lines = NULL;

        free(length);
        length = NULL;
        return NULL;
    }
    *my_length = length;
    return lines;
}

int read_file2(char* file_name, char*** my_lines, int* count, int** my_length) {
    __FILE * fp = NULL;
    int n = 2;
    int m = 2;
    char** lines = (char**)malloc(n*sizeof(char*));
    int* length = (int*)malloc(m*sizeof(int));
    size_t len = 0;
    int read;

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        for(int i = 0; i < n; i++) {
            free(lines[i]);
        }
        free(lines);
        lines = NULL;
        free(length);
        length = NULL;
        return -2;
    }
    int i = 0;
    while (1) {
        char * line = NULL;
        if((read = getline(&line, &len, fp)) != -1) {
            // printf("Retrieved line of length %zu/%d :%s", read, len, line);
            line[read] = NULL;
            lines[i] = line;
            length[i] = read;
            i++;
            if (i >= n) {
                char** tmp = (char**)realloc(lines, n*2*sizeof(char*));
                if (tmp) {
                    lines = tmp;
                    n *= 2;
                } else {
                    for(int i = 0; i < n; i++) {
                        free(lines[i]);
                    }
                    free(lines);
                    lines = NULL;
                    return -3;
                }
                tmp = NULL;
            }
            if (i >= m) {
                int* tmp = (int*)realloc(length, m*2*sizeof(int));
                if (tmp) {
                    length = tmp;
                    m *= 2;
                } else {
                    free(length);
                    length = NULL;
                    return -3;
                }
                tmp = NULL;
            }
        } else {
            break;
        }
    }
    *count = i;
    int r = fclose(fp);
    if (r) {
        for(int i = 0; i < n; i++) {
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

int one() {
    int count = 0;
    int* length;
    char** lines = read_file("./read.txt", &count, &length);
    if (NULL == lines) {
        printf("error\n");
        return -1;
    }
    printf("count = %d\n", count);
    for (int j = 0; j < count; j++) {
        printf("%d-%s", length[j], lines[j]);
    }
    return 0;
}

int two() {
    int count = 0;
    char** lines;
    int* length;
    int r = read_file2("./read.txt", &lines, &count, &length);
    if (r != 0) {
        printf("error\n");
        return -1;
    }
    printf("count = %d\n", count);
    for (int j = 0; j < count; j++) {
        printf("%d-%s", length[j], lines[j]);
    }
    return 0;
}

int main() {
    return one();
    // return two();
}
