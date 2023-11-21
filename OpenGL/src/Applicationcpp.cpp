#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// GL/glew.h 需要在 GLFW 等头文件之前

int main(void)
{
    GLFWwindow* window;

    /* 初始化库 */
    if (!glfwInit())
        return -1;

    /* 创建 window 和它的 OpenGL 上下文 */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /*  将指定窗口的 OpenGL 上下文设置为当前上下文
        在使用 OpenGL 进行图形渲染时，可以有多个窗口，每个窗口都有自己的 OpenGL 上下文
        上下文表示了 OpenGL 的当前状态，包括当前的渲染配置、状态设置等
        通过调用 glfwMakeContextCurrent 函数，你可以告诉 GLFW 使用指定窗口的上下文作为当前上下文，以便后续的 OpenGL 操作能够在该窗口上生效。
    */
    glfwMakeContextCurrent(window);

    // 必须在初始化上下文之后调用 glewInit
    if (glewInit() != GLEW_OK)
        std::cout << "glewInitError" << std::endl;

    // 当有一个有效的 OpenGL 上下文之后，可以输出版本
    // 4.6.14756 Compatibility Profile Context 20.40.52 27.20.14052.10
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    float position[6] =
    {
         0.5f,  0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer); // 如果没有成功 glewInit，运行相关函数时会抛出异常
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);  // 查看文档 docs.gl

    /* 循环直到关闭窗口 */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);   // 片元类型、顶点数组的起始索引、绘制多少个顶点


        /* 交换前后缓冲区 */
        glfwSwapBuffers(window);

        /* 处理轮询事件 */
        glfwPollEvents();
    }

    // 释放与 GLFW 相关的资源，并确保正确地关闭和清理 GLFW 库的状态
    glfwTerminate();
    return 0;
}