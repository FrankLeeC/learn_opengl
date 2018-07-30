#include <cglm/cglm.h>

#ifndef MYCOMMON_MODEL_H
#define MYCOMMON_MODEL_H

typedef struct Vertex 
{
    vec3 position;
    vec3 normal;
    vec2 texture;
} Vertex;

/**
 * 创建一个 Vertex
 */ 
Vertex* new_vertext(float x, float y, float z);

/**
 * 设置法线向量
 */ 
void set_normal(Vertex* v, float x, float y, float z);

/**
 * 设置贴图
 */ 
void set_texture(Vertex* v, float x, float y);



typedef struct Texture 
{
    unsigned int id;
    char* type;
} Texture;

/**
 * 创建一个 Texture
 */  
Texture* new_texture(unsigned int id, char* type);



typedef struct Mesh 
{
    Vertex** vertices;  // 顶点数组
    unsigned int v_count;  // 顶点个数
    unsigned int* indices;  // 顺序数组
    unsigned int idx_count;  // 顺序个数
    Texture** Textures;  // 纹理数组
    unsigned int t_count;  // 纹理个数
} Mesh;

/**
 * 创建一个 Mesh
 * 初始化的顶点个数，顺序个数，纹理个数都是 0
 */ 
Mesh* new_mesh();

void add_vertex(Vertex* v);

void add_index(unsigned int idx);

void add_texture(Texture* t);
#endif