#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// GL/glew.h 需要在 GLFW 等头文件之前

#include "GLError.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

struct ShaderCode
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderCode ReadShader(const std::string& filePath)
{
    enum class ShaderType 
    {
        NONE = -1,
        VERTEX = 0,  // 通过合理设值, 可以将枚举转换成整型, 用作数组下标
        FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;
    std::ifstream fstream(filePath);
    std::stringstream ss[2];
    if (!fstream.is_open())
    {
        std::cout << "Failed to open shader file" << std::endl;
    }
    std::string line;
    while (std::getline(fstream, line))
    {
        if (line.find("#shader") != std::string::npos) // 找到 shader
        {
            if (line.find("vertex") != std::string::npos) // vertex shader
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id; // 提升作用域
    GLCall(id = glCreateShader(type));  
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, 0));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char)); // alloca 才是在栈上申请内存, 且使用 malloc 时需要手动 free
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl; 
        GLCall(glDeleteShader(id));
        return 0; // 编译 shader 失败
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program;  // 提升作用域
    GLCall(program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);   // GL_VERTEX_SHADER 可以看做一个数, 直接作为参数传入
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));   // 似乎没有作用
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* 初始化库 */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    glfwSwapInterval(2); // 每等到 interval 个垂直同步信号后开始渲染新的一帧到后缓冲区, 如果为 0 则是关闭垂直同步, 帧率没有上限
                         // 60hz屏幕下, 1 为 60hz, 2 为 30hz, 3 为 20 hz

    // 必须在初始化上下文之后调用 glewInit
    if (glewInit() != GLEW_OK)
        std::cout << "glewInitError" << std::endl;

    // 当有一个有效的 OpenGL 上下文之后，可以输出版本
    // 4.6.14756 Compatibility Profile Context 20.40.52 27.20.14052.10
    unsigned char* glVersion;  // 提升作用域
    GLCall(glVersion = (unsigned char*)glGetString(GL_VERSION));
    std::cout << "Status: Using GL " << glVersion << std::endl;

    float position[] =
    {
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f
    };

    unsigned int indices[]
    {
        0, 1, 2,
        1, 2, 3
    };
    {
        VertexArray va;
        VertexBuffer vb(position, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        std::string filePath = "OpenGL/res/shaders/basic.shader";
        ShaderCode source = ReadShader(filePath);
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        int location;
        GLCall(location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.6f, 0.2f, 0.3f, 1.0f));

        // 解绑顶点数组, shader, 顶点缓冲, 索引缓冲
        va.Unbind();
        GLCall(glUseProgram(0));
        vb.Unbind();
        ib.Unbind();

        float r = 0.6f;
        float increment = 0.05f;

        /* 循环直到关闭窗口 */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // 只需要重新绑定 shader, 顶点数组 和 索引缓冲
            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.2f, 0.3f, 1.0f));

            va.Bind();
            ib.Bind();

            //glDrawArrays(GL_TRIANGLES, 0, 3);   // 片元类型、顶点数组的起始索引、绘制多少个顶点
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // 片元类型、索引个数、索引类型、索引缓冲区指针, 绑定了就不需要指定了
            if (r > 1.0f)
                increment = -0.05f;
            if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            /* 交换前后缓冲区 */
            glfwSwapBuffers(window);

            /* 处理轮询事件 */
            glfwPollEvents();
        }
        GLCall(glDeleteProgram(shader));
    }
    // 释放与 GLFW 相关的资源，并确保正确地关闭和清理 GLFW 库的状态
    glfwTerminate();
    return 0;
}