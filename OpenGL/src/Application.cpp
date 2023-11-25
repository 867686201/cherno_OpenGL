#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// GL/glew.h ��Ҫ�� GLFW ��ͷ�ļ�֮ǰ

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
        VERTEX = 0,  // ͨ��������ֵ, ���Խ�ö��ת��������, ���������±�
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
        if (line.find("#shader") != std::string::npos) // �ҵ� shader
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
    unsigned int id; // ����������
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
        char* message = (char*)alloca(length * sizeof(char)); // alloca ������ջ�������ڴ�, ��ʹ�� malloc ʱ��Ҫ�ֶ� free
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl; 
        GLCall(glDeleteShader(id));
        return 0; // ���� shader ʧ��
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program;  // ����������
    GLCall(program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);   // GL_VERTEX_SHADER ���Կ���һ����, ֱ����Ϊ��������
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));   // �ƺ�û������
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* ��ʼ���� */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* ���� window ������ OpenGL ������ */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /*  ��ָ�����ڵ� OpenGL ����������Ϊ��ǰ������
        ��ʹ�� OpenGL ����ͼ����Ⱦʱ�������ж�����ڣ�ÿ�����ڶ����Լ��� OpenGL ������
        �����ı�ʾ�� OpenGL �ĵ�ǰ״̬��������ǰ����Ⱦ���á�״̬���õ�
        ͨ������ glfwMakeContextCurrent ����������Ը��� GLFW ʹ��ָ�����ڵ���������Ϊ��ǰ�����ģ��Ա������ OpenGL �����ܹ��ڸô�������Ч��
    */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(2); // ÿ�ȵ� interval ����ֱͬ���źź�ʼ��Ⱦ�µ�һ֡���󻺳���, ���Ϊ 0 ���ǹرմ�ֱͬ��, ֡��û������
                         // 60hz��Ļ��, 1 Ϊ 60hz, 2 Ϊ 30hz, 3 Ϊ 20 hz

    // �����ڳ�ʼ��������֮����� glewInit
    if (glewInit() != GLEW_OK)
        std::cout << "glewInitError" << std::endl;

    // ����һ����Ч�� OpenGL ������֮�󣬿�������汾
    // 4.6.14756 Compatibility Profile Context 20.40.52 27.20.14052.10
    unsigned char* glVersion;  // ����������
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

        // ��󶥵�����, shader, ���㻺��, ��������
        va.Unbind();
        GLCall(glUseProgram(0));
        vb.Unbind();
        ib.Unbind();

        float r = 0.6f;
        float increment = 0.05f;

        /* ѭ��ֱ���رմ��� */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // ֻ��Ҫ���°� shader, �������� �� ��������
            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.2f, 0.3f, 1.0f));

            va.Bind();
            ib.Bind();

            //glDrawArrays(GL_TRIANGLES, 0, 3);   // ƬԪ���͡������������ʼ���������ƶ��ٸ�����
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // ƬԪ���͡������������������͡�����������ָ��, ���˾Ͳ���Ҫָ����
            if (r > 1.0f)
                increment = -0.05f;
            if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            /* ����ǰ�󻺳��� */
            glfwSwapBuffers(window);

            /* ������ѯ�¼� */
            glfwPollEvents();
        }
        GLCall(glDeleteProgram(shader));
    }
    // �ͷ��� GLFW ��ص���Դ����ȷ����ȷ�عرպ����� GLFW ���״̬
    glfwTerminate();
    return 0;
}