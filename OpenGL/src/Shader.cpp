#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "GLError.h"


Shader::Shader(const std::string& filepath)
	: m_RendererID(0), m_Filepath(filepath)
{
    ShaderCode source = ReadShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}


ShaderCode Shader::ReadShader(const std::string& filePath)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformCache.find(name) != m_UniformCache.end())
    {
        return m_UniformCache[name];
    }
    int location;
    GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: Uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformCache[name] = location;
    return location;
}

void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
{
    GLCall(glUniform4f(GetUniformLocation(name), v1, v2, v3, v4));
}
