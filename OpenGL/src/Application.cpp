#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>


// GL/glew.h ��Ҫ�� GLFW ��ͷ�ļ�֮ǰ

#include "GLError.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
        -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f
    };

    unsigned int indices[]
    {
        0, 1, 2,
        1, 2, 3
    };
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(position, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Renderer renderer;
        Texture texture("OpenGL/res/textures/ChernoLogo.png");
        Shader shader("OpenGL/res/shaders/basic.shader");
        
        glm::mat4 proj = glm::ortho(-2.0, 2.0, -1.5, 1.5, -1.0, 1.0);

        shader.Bind();
        texture.Bind();
        shader.SetUniform1i("u_Texture", texture.GetSlot());
        shader.SetUniformMat4fv("u_MVP", proj);

        /* ѭ��ֱ���رմ��� */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            // ֻ��Ҫ���°� shader, �������� �� ��������
            shader.Bind();
            va.Bind();
            ib.Bind();
            texture.Bind();
            renderer.Draw(va, ib, shader);

            /* ����ǰ�󻺳��� */
            glfwSwapBuffers(window);

            /* ������ѯ�¼� */
            glfwPollEvents();
        }
    }
    // �ͷ��� GLFW ��ص���Դ����ȷ����ȷ�عرպ����� GLFW ���״̬
    glfwTerminate();
    return 0;
}