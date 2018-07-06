#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mycommon/loadShader.h>
#include <cglm/cglm.h>
#include <stdio.h>

void processInput(GLFWwindow *window);
void processCursor(GLFWwindow *window, double x, double y);
void processZoom(GLFWwindow *window, double xoffset, double yoffset);

float last_time = 0.0f;
float delta = 0.0f;
float speed = 0.5f;
vec3 camera_pos = {0.0f, 0.0f, 3.0f};
vec3 camera_front = {0.0f, 0.0f, -1.0f};
vec3 camera_up = {0.0f, 1.0f, 0.0f};

double fov = 45.0f;

double lastx = 400;
double lasty = 300;
int first = 1;

float pitch = 0.0f;
float yaw = 0.0f;

int main(int argc, char** argv) {
    int init = glfwInit();
    if (!init) {
        printf("glfw init error\n");
        return 1;
    }
    glfwWindowHint(GL_MAJOR_VERSION, 4);
    glfwWindowHint(GL_MINOR_VERSION, 6);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Texture", NULL, NULL);
    if (NULL == window) {
        printf("failed to create window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("failed to initialize glad\n");
        return 1;
    }

    GLuint vshader, fshader;
    int a = get_shader("./source/shader/light.vert", GL_VERTEX_SHADER, &vshader);
    if (a != 0) {
        printf("failed to create vertex shader: %d\n", a);
        return 1;
    }
    
    a = get_shader("./source/shader/light.frag", GL_FRAGMENT_SHADER, &fshader);
    if (a != 0) {
        printf("failed to create fragment shader: %d\n", a);
        return 1;
    }

    GLuint* shaders = (GLuint*)malloc(2*sizeof(GLuint));
    shaders[0] = vshader;
    shaders[1] = fshader;
    GLuint light_program;

    a = get_program(2, shaders, &light_program);
    if (a != 0) {
        printf("create program error: %d\n", a);
    }
    free(shaders);


    a = get_shader("./source/shader/cube.vert", GL_VERTEX_SHADER, &vshader);
    if (a != 0) {
        printf("failed to create vertex shader: %d\n", a);
        return 1;
    }
    
    a = get_shader("./source/shader/cube.frag", GL_FRAGMENT_SHADER, &fshader);
    if (a != 0) {
        printf("failed to create fragment shader: %d\n", a);
        return 1;
    }

    shaders = (GLuint*)malloc(2*sizeof(GLuint));
    shaders[0] = vshader;
    shaders[1] = fshader;

    GLuint cube_program;
    a = get_program(2, shaders, &cube_program);
    if (a != 0) {
        printf("create program error: %d\n", a);
    }
    free(shaders);


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
    glGenBuffers(1, &vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);

    mat4 view, projection, light_model, cube_model;
    vec3 light_position = {1.2f, 1.0f, 2.0f};
    vec3 center;
    vec3 up = {0.0f, 1.0f, 0.0f};
    vec3 scale = {0.2f, 0.2f, 0.2f};
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glfwSetCursorPosCallback(window, processCursor);
        glfwSetScrollCallback(window, processZoom);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        glUseProgram(light_program);
        glm_mat4_identity(&view);
        glm_vec_add(camera_front, camera_pos, &center);
        glm_lookat(&camera_pos, &center, &up, &view);
        unsigned int light_view_loc = glGetUniformLocation(light_program, "view");
        glUniformMatrix4fv(light_view_loc, 1, GL_FALSE, &view[0]);
        glm_mat4_identity(&projection);
        glm_perspective(glm_rad(fov), (float)800 / (float)600, 0.1f, 100.0f, &projection);
        unsigned int light_projection_loc = glGetUniformLocation(light_program, "projection");
        glUniformMatrix4fv(light_projection_loc, 1, GL_FALSE, &projection[0]);
        glm_mat4_identity(&light_model);
        glm_translate(&light_model, &light_position);
        glm_scale(&light_model, &scale);
        unsigned int light_model_loc = glGetUniformLocation(light_program, "model");
        glUniformMatrix4fv(light_model_loc, 1, GL_FALSE, &light_model[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(vao2);
        glUseProgram(cube_program);
        glm_lookat(&camera_pos, &center, &up, &view);
        unsigned int cude_view_loc = glGetUniformLocation(cube_program, "view");
        glUniformMatrix4fv(cude_view_loc, 1, GL_FALSE, &view[0]);
        glm_mat4_identity(&projection);
        glm_perspective(glm_rad(fov), (float)800 / (float)600, 0.1f, 100.0f, &projection);
        unsigned int cube_projection_loc = glGetUniformLocation(cube_program, "projection");
        glUniformMatrix4fv(cube_projection_loc, 1, GL_FALSE, &projection[0]);
        glUniform3f(glGetUniformLocation(cube_program, "color"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(cube_program, "light"), 0.0f, 1.0f, 0.0f);
        glm_mat4_identity(&cube_model);
        unsigned int cube_model_loc = glGetUniformLocation(cube_program, "model");
        glUniformMatrix4fv(cube_model_loc, 1, GL_FALSE, &cube_model[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    float c = glfwGetTime();
    delta = c - last_time;
    last_time = c;
    float c_speed = delta * speed;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 camera_front_move;
        glm_vec_scale(&camera_front, c_speed, &camera_front_move);
        glm_vec_add(&camera_pos, &camera_front_move, &camera_pos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 camera_front_move;
        glm_vec_scale(&camera_front, -c_speed, &camera_front_move);
        glm_vec_add(&camera_pos, &camera_front_move, &camera_pos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 right;
        glm_cross(camera_up, camera_front, &right);
        glm_normalize(&right);
        glm_vec_scale(&right, c_speed, &right);
        glm_vec_add(&camera_pos, &right, &camera_pos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 right;
        glm_cross(camera_up, camera_front, &right);
        glm_normalize(&right);
        glm_vec_scale(&right, -c_speed, &right);
        glm_vec_add(&camera_pos, &right, &camera_pos);
    }
}

void processCursor(GLFWwindow *window, double x, double y) {
    if (first) {
        first = 0;
        lastx = x;
        lasty = y;
    }
    float xoffset = x - lastx;
    float yoffset = lasty - y;
    lastx = x;
    lasty = y;
    float sensitive = 0.05f;
    xoffset *= sensitive;
    yoffset *= sensitive;

    pitch += yoffset;
    yaw += xoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    vec3 f = {0.0f, 0.0f, 0.0f};
    f[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    f[1] = sin(glm_rad(pitch));
    f[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_normalize_to(&f, &camera_front);
}

void processZoom(GLFWwindow *window, double xoffset, double yoffset) {
    if (fov >= 1.0f && fov <= 45.0f) {
        fov -= yoffset;
    }
    if (fov <= 1.0f) {
        fov = 1.0f;
    }
    if (fov >= 45.0f) {
        fov = 45.0f;
    }
}