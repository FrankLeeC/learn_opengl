#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
    
    // 创建一个shader
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    if (!shader) {
        printf("failed to create shader\n");
        return 1;
    }

    
    
}