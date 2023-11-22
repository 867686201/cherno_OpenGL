#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// GL/glew.h 需要在 GLFW 等头文件之前

#define ASSERT(x) if(!x) __debugbreak();   // 断言, 如果为 false, 则调用 msvc 的断点

//  \ 是换行转义符, 之后不能加空格
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall()) 
 
static void GLClearError()   // 循环获取错误, 即获取所有错误, 则清空错误了
{
    while (glGetError() != GL_NO_ERROR);  
}

static bool GLLogCall()   // 循环获取错误, 每获取到错误, 就输出错误码, 获取错误过程遵循先入先出原则, 即先发生的错误先获取
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << std::hex << error << ")" << std::endl;   // 由于错误码是 16 进制, 通过 hex 输出16进制数
        return false;     // 如果有错误, 则返回 false
    }
    return true;
}

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
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // alloca 才是在栈上申请内存, 且使用 malloc 时需要手动 free
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl; 
        glDeleteShader(id);
        return 0; // 编译 shader 失败
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);   // GL_VERTEX_SHADER 可以看做一个数, 直接作为参数传入
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);   // 似乎没有作用
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}


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

    unsigned int buffer;
    glGenBuffers(1, &buffer); // 如果没有成功 glewInit，运行相关函数时会抛出异常
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), position, GL_STATIC_DRAW);  // 查看文档 docs.gl

    glEnableVertexAttribArray(0);
    /**
     * glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
     * @param index, 属性的索引, 如位置在索引0，纹理在索引1，法线在索引2
     * @param size, 该属性的 count, 每个顶点的某个属性有多少个 count, 如这里每个顶点的位置属性有 2 个
     * @param type, 数据类型
     * @param normalized, 只用于整数, 归一化到 [0, 1] 或 [-1, 1] (根据是否是有符号整数)
     * @param stride, 整个顶点的字节大小
     * @param pointer, 属性 offset 的字节大小
     */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    std::string filePath = "OpenGL/res/shaders/basic.shader";

    ShaderCode source = ReadShader(filePath);

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    

    glUseProgram(shader);


    /* 循环直到关闭窗口 */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);   // 片元类型、顶点数组的起始索引、绘制多少个顶点
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)); // 片元类型、索引个数、索引类型、索引缓冲区指针, 绑定了就不需要指定了

        /* 交换前后缓冲区 */
        glfwSwapBuffers(window);

        /* 处理轮询事件 */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    // 释放与 GLFW 相关的资源，并确保正确地关闭和清理 GLFW 库的状态
    glfwTerminate();
    return 0;
}