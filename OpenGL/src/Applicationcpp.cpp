#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// GL/glew.h ��Ҫ�� GLFW ��ͷ�ļ�֮ǰ

#define ASSERT(x) if(!x) __debugbreak();   // ����, ���Ϊ false, ����� msvc �Ķϵ�

//  \ �ǻ���ת���, ֮���ܼӿո�
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall()) 
 
static void GLClearError()   // ѭ����ȡ����, ����ȡ���д���, ����մ�����
{
    while (glGetError() != GL_NO_ERROR);  
}

static bool GLLogCall()   // ѭ����ȡ����, ÿ��ȡ������, �����������, ��ȡ���������ѭ�����ȳ�ԭ��, ���ȷ����Ĵ����Ȼ�ȡ
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << std::hex << error << ")" << std::endl;   // ���ڴ������� 16 ����, ͨ�� hex ���16������
        return false;     // ����д���, �򷵻� false
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
        char* message = (char*)alloca(length * sizeof(char)); // alloca ������ջ�������ڴ�, ��ʹ�� malloc ʱ��Ҫ�ֶ� free
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl; 
        glDeleteShader(id);
        return 0; // ���� shader ʧ��
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);   // GL_VERTEX_SHADER ���Կ���һ����, ֱ����Ϊ��������
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);   // �ƺ�û������
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* ��ʼ���� */
    if (!glfwInit())
        return -1;
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

    // �����ڳ�ʼ��������֮����� glewInit
    if (glewInit() != GLEW_OK)
        std::cout << "glewInitError" << std::endl;

    // ����һ����Ч�� OpenGL ������֮�󣬿�������汾
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
    glGenBuffers(1, &buffer); // ���û�гɹ� glewInit��������غ���ʱ���׳��쳣
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), position, GL_STATIC_DRAW);  // �鿴�ĵ� docs.gl

    glEnableVertexAttribArray(0);
    /**
     * glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
     * @param index, ���Ե�����, ��λ��������0������������1������������2
     * @param size, �����Ե� count, ÿ�������ĳ�������ж��ٸ� count, ������ÿ�������λ�������� 2 ��
     * @param type, ��������
     * @param normalized, ֻ��������, ��һ���� [0, 1] �� [-1, 1] (�����Ƿ����з�������)
     * @param stride, ����������ֽڴ�С
     * @param pointer, ���� offset ���ֽڴ�С
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


    /* ѭ��ֱ���رմ��� */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);   // ƬԪ���͡������������ʼ���������ƶ��ٸ�����
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)); // ƬԪ���͡������������������͡�����������ָ��, ���˾Ͳ���Ҫָ����

        /* ����ǰ�󻺳��� */
        glfwSwapBuffers(window);

        /* ������ѯ�¼� */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    // �ͷ��� GLFW ��ص���Դ����ȷ����ȷ�عرպ����� GLFW ���״̬
    glfwTerminate();
    return 0;
}