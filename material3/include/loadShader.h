#include <glad/glad.h>

#ifndef _LOAD_SHADER_H
#define _LOAD_SHADER_H

/**
 * param:
 *   filename:  char*     filename
 *   type:      GLenum    type of shader
 *   my_shader: GLuint*   store shader result
 *  
 * return:
 *   -1:  create shader error
 *   -2:  read file error
 *   -3:  compile shader error
 *   0:   success
 */ 
int get_shader(char* filename, GLenum type, GLuint* my_shader);

/**
 * param:
 *   count:       int        count of shaders
 *   shaders:     GLuint*    shader array
 *   my_program:  GLuint*    store program result
 * 
 * return:
 *   -1:  create program error
 *   -2:  link program error
 *   -3:  one of shaders is not a shader.    glIsShader(shaders[i]) is false
 *   0:   success
 */ 
int get_program(int count, GLuint* shaders, GLuint* my_program);

#endif
