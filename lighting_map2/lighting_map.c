#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mycommon/camera.h>
#include <mycommon/loadShader.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stb/stb_image.h>

void processInput(GLFWwindow *window);
void processCursor(GLFWwindow *window, double x, double y);
void processZoom(GLFWwindow *window, double xoffset, double yoffset);
GLuint create_texture(const char* path, int *result);

float last_time = 0.0f;

double lastx = 400;
double lasty = 300;
int first = 1;

struct Camera *camera;

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
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };


    float t_normals[36][3] = {{0}};

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
    glGenBuffers(1, &vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glEnable(GL_DEPTH_TEST);

    mat4 projection, light_model, cube_model;
    vec3 light_position = {0.5f, 0.2f, 0.7f};
    vec3 scale = {0.2f, 0.2f, 0.2f};
    vec3 cube_scale = {0.8f, 0.6f, 0.7f};

    float light[3][3] = {
        {0.2f, 0.2f, 0.2f},
        {0.5f, 0.5f, 0.5f},
        {1.0f, 1.0f, 1.0f}
    };
    float light_color[3] = {0.0f};
    float shininess = 64.0f;

    GLuint texture1 = create_texture("./source/texture/container2.png", &a);
    if (a != 0) {
        return -1;
    }
    GLuint texture2 = create_texture("./source/texture/container2_specular.png", &a);
    if (a != 0) {
        return -1;
    }
    glUseProgram(cube_program);
    glUniform1i(glGetUniformLocation(cube_program, "material.ambient"), 0);
    glUniform1i(glGetUniformLocation(cube_program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(cube_program, "material.specular"), 1);
    camera = init_camera();
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glfwSetCursorPosCallback(window, processCursor);
        glfwSetScrollCallback(window, processZoom);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        glUseProgram(light_program);
        GLfloat *view = camera_lookat(camera);
        unsigned int light_view_loc = glGetUniformLocation(light_program, "view");
        glUniformMatrix4fv(light_view_loc, 1, GL_FALSE, view);
        glm_mat4_identity(&projection);
        glm_perspective(glm_rad(camera->fov), (float)800 / (float)600, 0.1f, 100.0f, &projection);
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
        unsigned int cude_view_loc = glGetUniformLocation(cube_program, "view");
        glUniformMatrix4fv(cude_view_loc, 1, GL_FALSE, view);
        glm_mat4_identity(&projection);
        glm_perspective(glm_rad(camera->fov), (float)800 / (float)600, 0.1f, 100.0f, &projection);
        unsigned int cube_projection_loc = glGetUniformLocation(cube_program, "projection");
        glUniformMatrix4fv(cube_projection_loc, 1, GL_FALSE, &projection[0]);

        glUniform3fv(glGetUniformLocation(cube_program, "light.ambient"), 1, light[0]);
        glUniform3fv(glGetUniformLocation(cube_program, "light.diffuse"), 1, light[1]);
        glUniform3fv(glGetUniformLocation(cube_program, "light.specular"), 1, light[2]);
        glUniform3fv(glGetUniformLocation(cube_program, "light.position"), 1, light_position);

        glm_mat4_identity(&cube_model);
        glm_scale(&cube_model, &cube_scale);
        unsigned int cube_model_loc = glGetUniformLocation(cube_program, "model");
        glUniformMatrix4fv(cube_model_loc, 1, GL_FALSE, &cube_model[0]);
        glUniform3fv(glGetUniformLocation(cube_program, "view_position"), 1, camera->position);

        mat4 inv_model;
        glm_mat4_inv(&cube_model, &inv_model);
        glm_mat4_transpose(&inv_model);
        mat3 tmp;
        glm_mat4_pick3(&inv_model, &tmp);
        glUniformMatrix3fv(glGetUniformLocation(cube_program, "normal_trans"), 1, GL_FALSE, &tmp[0]);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1f(glGetUniformLocation(cube_program, "material.shininess"), shininess);

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
    float delta = c - last_time;
    last_time = c;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_move(camera, MOVE_FORWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_move(camera, MOVE_BACK, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_move(camera, MOVE_LEFT, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_move(camera, MOVE_RIGHT, delta);
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
    camera_rotate(camera, xoffset, yoffset);
}

void processZoom(GLFWwindow *window, double xoffset, double yoffset) {
    camera_zoom(camera, xoffset, yoffset);
}

GLuint create_texture(const char* path, int *result) {
    *result = 1;
    GLuint texture;
    glGenTextures(1, &texture);
    int width, height, channel;
    char *data = stbi_load(path, &width, &height, &channel, 0);
    if (data) {
        GLenum format;
        if (channel == 1)
            format = GL_RED;
        else if (channel == 3)
            format = GL_RGB;
        else if (channel == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        *result = 0;
    } else {
        printf("failed to create texture: %s\n", path);
    }
    stbi_image_free(data);
    return texture;
}