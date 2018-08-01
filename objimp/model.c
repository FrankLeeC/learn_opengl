#include "model.h"
#include <stdlib.h>


Vertex* init_vertext(float x, float y, float z)
{
    Vertex* v = (Vertex*)malloc(sizeof(Vertex));
    v->position[0] = x;
    v->position[1] = y;
    v->position[2] = z;
    return v;
}

void set_normal(Vertex* v, float x, float y, float z)
{
    v->normal[0] = x;
    v->normal[1] = y;
    v->normal[2] = z;
}


void set_texture(Vertex* vt, float u, float v)
{
    vt->texture[0] = u;
    vt->texture[1] = v;
}


Texture* init_texture(unsigned int id, char* type)
{
    Texture* t = (Texture*)malloc(sizeof(Texture));
    t->id = id;
    t->type = type;
    return t;
}



Mesh* init_mesh()
{
    Mesh* m = (Mesh*)malloc(sizeof(Mesh));
    m->v_count = 0;
    m->idx_count = 0;
    m->t_count = 0;
    return m;
}

int add_vertex(Mesh* m, Vertex* v)
{
    if (m->v_count == 0) 
    {
        m->v_cap = 1 << 5;
        m->vertices = (Vertex**)malloc(sizeof(Vertex*));
        m->vertices[m->v_count++] = v;
        return 1;
    }
    if (m->v_count >= m->v_cap) 
    {
        m->v_cap <<= 2;
        Vertex** tmp = (Vertex**)realloc(m->vertices, m->v_cap * sizeof(Vertex*));
        if (!tmp) 
        {
            return 0;
        }
        if (tmp == m->vertices) 
        {
            m->vertices[m->v_count++] = v;
            return 1;
        }
        for (int i = 0; i < m->v_count; i++) {
            free(m->vertices[i]);
        }
        free(m->vertices);
        m->vertices = tmp;
        tmp = NULL;
        m->vertices[m->v_count++] = v;
        return 1;
    }
    m->vertices[m->v_count++] = v;
    return 1;
}

int get_vertex_count(Mesh* m)
{
    return m->v_count;
}

int add_index(Mesh* m, unsigned int idx)
{
    if (m->idx_count == 0) 
    {
        m->idx_cap = 1 << 5;
        m->indices = (unsigned int*)malloc(sizeof(unsigned int));
        m->indices[m->idx_count++] = idx;
        return 1;
    }
    if (m->idx_count >= m->idx_cap) 
    {
        m->idx_cap <<= 2;
        unsigned int* tmp = (unsigned int*)realloc(m->indices, m->idx_cap * sizeof(unsigned int));
        if (!tmp) 
        {
            return 0;
        }
        if (tmp == m->indices) 
        {
            m->indices[m->idx_count++] = idx;
            return 1;
        }
        for (int i = 0; i < m->idx_count; i++) {
            free(m->indices[i]);
        }
        free(m->indices);
        m->indices = tmp;
        tmp = NULL;
        m->indices[m->idx_count++] = idx;
        return 1;
    }
    m->indices[m->idx_count++] = idx;
    return 1;
}

int get_indicies_count(Mesh* m)
{
    return m->idx_count;
}

int add_texture(Mesh* m, Texture* t)
{
    if (m->t_count == 0) 
    {
        m->t_cap = 1 << 5;
        m->textures = (Texture**)malloc(sizeof(Texture**));
        m->textures[m->t_count++] = t;
        return 1;
    }
    if (m->t_count >= m->t_cap) 
    {
        m->t_cap <<= 2;
        Texture** tmp = (Texture**)realloc(m->indices, m->t_cap * sizeof(Texture**));
        if (!tmp) 
        {
            return 0;
        }
        if (tmp == m->textures) 
        {
            m->textures[m->t_count++] = t;
            return 1;
        }
        for (int i = 0; i < m->t_count; i++) {
            free(m->textures[i]);
        }
        free(m->textures);
        m->textures = tmp;
        tmp = NULL;
        m->textures[m->t_count++] = t;
        return 1;
    }
    m->indices[m->t_count++] = t;
    return 1;
}

int get_texture_count(Mesh* m)
{
    return m->t_count;
}

Vertex* get_vertex(Mesh* m, unsigned int index)
{
    if (index >= m->v_count) 
    {
        return NULL;
    }
    return m->vertices[index];
}

Texture* get_texture(Mesh* m, unsigned int index)
{
    if (index >= m->t_count)
    {
        return NULL;
    }
    return m->textures[index];
}