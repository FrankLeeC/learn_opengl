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
float mixValue = 0.2f;
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
    int a = get_shader("./source/shader/texture.vert", GL_VERTEX_SHADER, &vshader);
    if (a != 0) {
        printf("failed to create vertex shader: %d\n", a);
        return 1;
    }
    
    a = get_shader("./source/shader/texture.frag", GL_FRAGMENT_SHADER, &fshader);
    if (a != 0) {
        printf("failed to create fragment shader: %d\n", a);
        return 1;
    }

    GLuint* shaders = (GLuint*)malloc(2*sizeof(GLuint));
    shaders[0] = vshader;
    shaders[1] = fshader;
    GLuint program;

    a = get_program(2, shaders, &program);
    if (a != 0) {
        printf("create program error: %d\n", a);
    }

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vec3 positions[] = {
        {0.0f, 0.0f, 0.0f},
        {2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);  // 设置x轴纹理方式
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);  // 设置y轴纹理方式
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // 设置缩小时的纹理过滤
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 设置放大时的纹理过滤

    

    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load("./source/texture/container.jpg", &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(GL_TEXTURE_2D);
    } else {
        printf("failed to load texture\n");
        return 1;
    }
    stbi_image_free(data);

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTextureParameteri(texture2, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture2, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture2, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture2, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(1);
    data = stbi_load("./source/texture/awesomeface.png", &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(GL_TEXTURE_2D);
    } else {
        printf("failed to load texture2\n");
        return 1;
    }
    stbi_image_free(data);

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(program, "texture2"), 1);

    glEnable(GL_DEPTH_TEST);

    mat4 view, projection;
    vec3 v = {1.0f, 0.3f, 0.5f};
    vec3 center;
    vec3 up = {0.0f, 0.8f, 0.0f};
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glfwSetCursorPosCallback(window, processCursor);
        glfwSetScrollCallback(window, processZoom);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
            
        glm_mat4_identity(&view);
        glm_vec_add(camera_front, camera_pos, &center);
        glm_lookat(&camera_pos, &center, &up, &view);
        unsigned int viewLoc = glGetUniformLocation(program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0]);

        glm_mat4_identity(&projection);
        glm_perspective(glm_rad(fov), (float)800 / (float)600, 0.1f, 100.0f, &projection);
        unsigned int projectionLoc = glGetUniformLocation(program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0]);

        glUniform1f(glGetUniformLocation(program, "mixValue"), mixValue);

        glUseProgram(program);
        glBindVertexArray(vao);
        for (unsigned int i = 0 ; i < 10; i++) {
            mat4 model;
            glm_mat4_identity(&model);
            glm_translate(&model, &positions[i]);
            glm_rotate(&model, glm_rad(20.0f*(i+1))*(float)glfwGetTime(), &v);
            unsigned int modelLoc = glGetUniformLocation(program, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        vec3 camera_front_move;
        glm_vec_scale(&camera_front, c_speed, &camera_front_move);
        glm_vec_add(&camera_pos, &camera_front_move, &camera_pos);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        vec3 camera_front_move;
        glm_vec_scale(&camera_front, -c_speed, &camera_front_move);
        glm_vec_add(&camera_pos, &camera_front_move, &camera_pos);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        vec3 right;
        glm_cross(camera_up, camera_front, &right);
        glm_normalize(&right);
        glm_vec_scale(&right, c_speed, &right);
        glm_vec_add(&camera_pos, &right, &camera_pos);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
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