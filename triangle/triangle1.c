#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mycommon/loadShader.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int init = glfwInit();
    if (!init) {
        printf("glfw init error\n");
        return 1;
    }
    glfwWindowHint(GL_MAJOR_VERSION, 4);
    glfwWindowHint(GL_MINOR_VERSION, 6);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);
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
    // 创建 vertext shader
    int a = get_shader("./source/shader/triangle1.vert", GL_VERTEX_SHADER, &vshader);
    if (a != 0) {
        printf("failed to create vertex shader: %d\n", a);
        return 1;
    }
    
    // 创建 fragment shader
    a = get_shader("./source/shader/triangle1.frag", GL_FRAGMENT_SHADER, &fshader);
    if (a != 0) {
        printf("failed to create fragment shader: %d\n", a);
        return 1;
    }

    GLuint* shaders = (GLuint*)malloc(2*sizeof(GLuint));
    shaders[0] = vshader;
    shaders[1] = fshader;
    GLuint program;

    // link program with shaders
    a = get_program(2, shaders, &program);
    if (a != 0) {
        printf("create program error: %d\n", a);
    }

    glUseProgram(program);

    // 坐标
    float vertices[] = {
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vao);


    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        // 更新 uniform 
        float timev = glfwGetTime();
        float green = sin(timev) / 2.0f + 0.5f;
        GLint vcolor = glGetUniformLocation(program, "my_color");
        glUniform4f(vcolor, 0.0f, green, 0.0f, 1.0f);

        // 渲染
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
    
}

// gcc ./triangle1.c -lopengl32 -l:libglad.so -lglfw3dll -l:libshader.so