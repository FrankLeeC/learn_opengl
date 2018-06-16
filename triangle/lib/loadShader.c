// #define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
// #include <mycommon/loadShader.h>

int read_file(char* file_name, char*** my_lines, int* count, int** my_length) {
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
            line[read] = '\0';
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

int get_shader(char* filename, GLenum type, GLuint* my_shader) {
    GLuint shader = glCreateShader(type);
    if (!shader) {
        return -1;  // create shader error
    }

    int count = 0;
    char** lines;
    int* length;
    int r = read_file(filename, &lines, &count, &length);
    if (r != 0) {
        for (int i = 0; i < count; i++) {
            free(lines[i]);
        }
        free(lines);
        free(length);
        return -2;  // read file error
    }

    glShaderSource(shader, count, lines, length);

    for (int i = 0; i < count; i++) {
        free(lines[i]);
    }
    free(lines);
    free(length);
 
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);  // 获取shader的信息，第二个参数指定需要什么信息，第三个值返回信息的值
    if (!compiled) {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        GLchar* log_info = (char*)malloc(len*sizeof(char));
        glGetShaderInfoLog(shader, len, &len, log_info);
        printf("failed to compile shader: %s\n", log_info);
        free(log_info);

        glDeleteShader(shader);
        return -3;  // compile shader error
    }
    *my_shader = shader;
    return 0;
}

int get_program(int count, GLuint* shaders, GLuint* my_program) {
    GLuint program = glCreateProgram();
    if (!program) {
        return -1; // create program error
    }

    for (int i = 0; i < count; i++) {
        if (glIsShader(shaders[i])) {
            glAttachShader(program, shaders[i]);
        } else {
            return -3;
        }
    }
    glLinkProgram(program);

    int linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        int len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        char* log_info = (char*)malloc(len*sizeof(char));
        glGetProgramInfoLog(program, len, &len, log_info);
        printf("link program error: %s\n", log_info);
        free(log_info);
        for (int i = 0; i < count; i++) {
            glDetachShader(program, shaders[i]);
        }
        glDeleteProgram(program);
        return -2; // link program error
    }

    *my_program = program;
    return 0;
}

// gcc -o ./loadShader.o -c ./loadShader.c
// gcc -shared -fPIC -o libshader.so -L. -lglad ./loadShader.o