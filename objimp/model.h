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
Vertex* init_vertext(float x, float y, float z);

/**
 * 设置法线向量
 */ 
void set_normal(Vertex* v, float x, float y, float z);

/**
 * 设置贴图
 */ 
void set_texture(Vertex* vt, float u, float v);



typedef struct Texture 
{
    unsigned int id;
    char* type;
} Texture;

/**
 * 创建一个 Texture
 */  
Texture* init_texture(unsigned int id, char* type);



typedef struct Mesh 
{
    Vertex** vertices;  // 顶点数组
    unsigned int v_count;  // 顶点个数
    unsigned int v_cap;
    unsigned int* indices;  // 顺序数组
    unsigned int idx_count;  // 顺序个数
    unsigned int idx_cap;
    Texture** textures;  // 纹理数组
    unsigned int t_count;  // 纹理个数
    unsigned int t_cap;
} Mesh;

/**
 * 创建一个 Mesh
 * 初始化的顶点个数，顺序个数，纹理个数都是 0
 */ 
Mesh* init_mesh();

/**
 * 添加一个顶点
 * return: 1： 成功   0：失败
 */ 
int add_vertex(Mesh* m, Vertex* v);

/**
 * 获取顶点个数
 */ 
int get_vertex_count(Mesh* m);

/**
 * 添加一个绘制索引
 * return: 1： 成功   0：失败
 */ 
int add_index(Mesh* m, unsigned int idx);

/**
 * 获取顶点索引个数
 */ 
int get_indicies_count(Mesh* m);

/**
 * 添加一个贴图
 * return: 1： 成功   0：失败
 */ 
int add_texture(Mesh* m, Texture* t);

/**
 * 获取材质个数
 */ 
int get_texture_count(Mesh* m);

/**
 * 根据索引获取 Vertex*
 * 如果 index >= v_count 返回 NULL
 */ 
Vertex* get_vertex(Mesh* m, unsigned int index);

/**
 * 根据索引获取 Texture*
 * 如果 index >= t_count 返回 NULL
 */ 
Texture* get_texture(Mesh* m, unsigned int index);
#endif