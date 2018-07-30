#include <stdlib.h>
#include <string.h>
#include <strutil.h>

void trim(char **s) {
    if (NULL == s) {
        **s = '\0';
        return;
    }
    char *end = *s + strlen(*s) - 1;
    char *begin = *s;
    while (begin < end) {
        if (*begin == ' ' || *begin == '\t') {
            begin++;
            continue;
        }
        if (*end == ' ' || *end == '\t') {
            end--;
            continue;
        }
        break;
    }
    if (begin == end) {
        **s = '\0';
        return ;
    }
    *s = begin;
    end[1] = '\0';
}

void trim_to(char **out, char *in) {
    if (NULL == in) {
        **out = '\0';
        return;
    }
    printf("%d, %d\n", NULL == out, NULL == *out);
    if (*out == NULL) {
        *out = (char*)malloc(sizeof(in));
    }
    char *end = in + strlen(in) - 1;
    char *begin = in;
    while (begin < end) {
        if (*begin == ' ' || *begin == '\t') {
            begin++;
            continue;
        }
        if (*end == ' ' || *end == '\t') {
            end--;
            continue;
        }
        break;
    }
    if (begin == end) {
        **out = '\0';
        return;
    }
    memcpy(*out, begin, (end-begin+1)*sizeof(char));
}


void split(char ***out, char *in, char* sep, int *count) {
    int a = 0, b = 1 << 1;
    int sep_len = strlen(sep);
    int *indices = (int*)malloc(b*sizeof(int));
    int len = strlen(in);
    if (len == 0) {
        *count = 0;
        return;
    }
    if (sep_len > len) {  // 分隔符长度大于输入字符长度
        *count = 0;
        return;
    }
    if (sep_len == 0) {
        *count = 1;
        char** my_out = (char**)malloc(1*sizeof(char*));
        char* tmp = (char*) malloc(len*sizeof(char));
        memcpy(tmp, in, len);
        my_out[0] = tmp;
        *out = my_out;
        return;
    }

    for (int i = 0; i < len; i++) {
        int match = 1;
        if (in[i] == sep[0]) {
            for (int j = 1; j < sep_len; j++) {
                if (in[i+j] != sep[j]) {
                    match = 0;
                    break;
                }
            }
            if (!match) {
                continue;
            }

            if (a >= b - 1) {
                b <<= 1;
                int *tmp = (int*)realloc(indices, b*sizeof(int));
                if (tmp != NULL) {
                    indices = tmp;
                }
            }
            indices[a++] = i;

            i += sep_len - 1;
        }
    }
    if (a == 0) {
        char **my_out = (char**)malloc(sizeof(char*));
        char *tmp = (char*)malloc((len+1)*sizeof(char));
        memcpy(tmp, in, len*sizeof(char*));
        tmp[len] = '\0';
        my_out[0] = tmp;
        *out = my_out;
        *count = 1;
        return;
    }
    *count = a + 1;
    char **my_out = (char**)malloc((*count) * sizeof(char*));
    for (int i = 0; i < a; i++) {
        char *begin;
        int size;
        if (i == 0) {
            begin = in;
            size = indices[i] * sizeof(char);
        } else {
            begin = in + indices[i-1] + sep_len;
            size = (indices[i] - indices[i-1] - sep_len) * sizeof(char);
        }
        char *tmp = (char*)malloc((size+1)*sizeof(char));
        memcpy(tmp, begin, size);
        tmp[size] = '\0';
        my_out[i] = tmp;
    }
    if (indices[a-1] < len - 1) {
        int size = len - 1 - indices[a - 1];
        char *tmp = (char*)malloc((size+1)*sizeof(char));
        memcpy(tmp, in + indices[a - 1] + sep_len, size);
        tmp[size] = '\0';
        my_out[a] = tmp;
    } else {
        char *tmp = (char*)malloc(1);
        tmp[0] = '\0';
        my_out[a] = tmp;
    }
    *out = my_out;
}