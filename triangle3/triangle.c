#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mycommon/loadShader.h>
#include <stdio.h>


int main(int argc, char **argv) {
    int init = glfwInit();
    if (!init) {
        printf("glfw init error\n");
        return 1;
    }
    glfwWindowHint(GL_MAJOR_VERSION, 4);
    glfwWindowHint(GL_MINOR_VERSION, 6);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);
    if (NULL == window) {
        printf("failed to create window\n");
        glfwTerminate();
        return 1;
    }

    // 设置为当前窗口
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader(glfwGetProcAddress)) {
        printf("failed to initialize glad\n");
        return 1;
    }

    GLuint vshader, fshader;
    int a = get_shader("./source/shader/triangle.vert", GL_VERTEX_SHADER, &vshader);
    if (a != 0) {
        printf("failed to create vertex shader: %d\n", a);
        return 1;
    }
    a = get_shader("./source/shader/triangle.frag", GL_FRAGMENT_SHADER, &fshader);
    if (a != 0) {
        printf("failed to create fragment shader: %d\n", a);
        return 1;
    }

    GLuint *shaders = (GLuint*)malloc(2*sizeof(GLuint));
    shaders[0] = vshader;
    shaders[1] = fshader;
    GLuint program;
    a = get_program(2, shaders, &program);
    if (a != 0) {
        printf("create program error: %d\n", a);
    }

    GLuint vshader2, fshader2;
    a = get_shader("./source/shader/triangle.1.vert", GL_VERTEX_SHADER, &vshader2);
    if (a != 0) {
        printf("failed to create vertex shader2: %d\n", a);
        return 1;
    }
    a = get_shader("./source/shader/triangle.1.frag", GL_FRAGMENT_SHADER, &fshader2);
    if (a != 0 ) {
        printf("failed to create fragment shader2: %d\n", a);
        return 1;
    }
    GLuint *shaders2 = (GLuint*)malloc(2*sizeof(GLuint));
    shaders2[0] = vshader2;
    shaders2[1] = fshader2;
    GLuint program2;
    a = get_program(2, shaders2, &program2);
    if (a != 0) {
        printf("failed to create program2: %d\n", a);
        return 1;
    }

    float vertices[] = {
        0.3f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
    };
    GLuint vbo[2], vao[2];
    glGenVertexArrays(2, &vao[0]);
    glBindVertexArray(vao[0]);
    glGenBuffers(2, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 3*sizeof(float));;
    glEnableVertexAttribArray(1);

    float vertices2[] = {
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.3f, 0.5f, 0.0f
    };
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 渲染第一个
        glBindVertexArray(vao[0]);
        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 渲染第二个
        glBindVertexArray(vao[1]);
        glUseProgram(program2);
        GLuint color = glGetUniformLocation(program2, "my_color");
        glUniform4f(color, 0.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(2, &vao);
    glDeleteBuffers(2, &vbo);

    glfwTerminate();
    return 0;
    
}
