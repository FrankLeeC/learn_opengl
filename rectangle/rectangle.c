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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Rectangle", NULL, NULL);
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
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint vbo, vao, ibo;

    // 创建并绑定顶点对象，后续操作都会对这个对象起作用
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ibo);

    // 创建并绑定缓存
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // 传输缓存数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性
    // 第一个参数表示vert中layout值对应的顶点
    // 第二个参数表示每个点的数据个数 1/2/3/4
    // 第三个参数表示顶点数据类型
    // 第四个参数表示是否归一化，FALSE表示不归一化。直接将整型转为浮点型
    // 第五个参数表示每两个连续元素之间的偏移字节数
    // 第六个参数表示起始数据在数组中的偏移
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 启用顶点数组对象
    // 参数index与glVertextAttribPointer第一个参数index保持一致
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        // // 更新 uniform 
        // float timev = glfwGetTime();
        // float green = sin(timev) / 2.0f + 0.5f;
        // GLint vcolor = glGetUniformLocation(program, "my_color");
        // glUniform4f(vcolor, 0.0f, green, 0.0f, 1.0f);

        // // 渲染
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);

    glfwTerminate();
    return 0;
    
}

// gcc ./rectangle.c -lopengl32 -l:libglad.so -lglfw3dll -l:libshader.so