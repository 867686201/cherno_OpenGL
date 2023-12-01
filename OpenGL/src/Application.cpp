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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


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
    window = glfwCreateWindow(960, 600, "Hello World", NULL, NULL);
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

    //glfwSwapInterval(2); // ÿ�ȵ� interval ����ֱͬ���źź�ʼ��Ⱦ�µ�һ֡���󻺳���, ���Ϊ 0 ���ǹرմ�ֱͬ��, ֡��û������
                         // 60hz��Ļ��, 1 Ϊ 60hz, 2 Ϊ 30hz, 3 Ϊ 20 hz

    // �����ڳ�ʼ��������֮����� glewInit
    if (glewInit() != GLEW_OK)
        std::cout << "glewInitError" << std::endl;

    // ����һ����Ч�� OpenGL ������֮�󣬿�������汾
    // 4.6.14756 Compatibility Profile Context 20.40.52 27.20.14052.10
    unsigned char* glVersion;  // ����������
    GLCall(glVersion = (unsigned char*)glGetString(GL_VERSION));
    std::cout << "Status: Using GL " << glVersion << std::endl;

    float vertices[] =
    {
        100.0f, 100.0f, 0.0f, 0.0f,
        100.0f, 200.0f, 0.0f, 1.0f,
        200.0f, 100.0f, 1.0f, 0.0f,
        200.0f, 200.0f, 1.0f, 1.0f
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
        VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 2 * 3);

        Renderer renderer;
        Texture texture("OpenGL/res/textures/ChernoLogo.png");
        Shader shader("OpenGL/res/shaders/basic.shader");
        
        glm::vec3 translation(100.0f, 100.0f, 0.0f);
        glm::vec3 eye(0.0f, 0.0f, 400.0f);
        glm::vec3 center(0.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 view = glm::lookAt(eye, center, up);
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 600.0f, -1000.0f, 1000.0f);

        glm::mat4 mvp = proj * view * model;

        shader.Bind();
        texture.Bind();
        shader.SetUniform1i("u_Texture", texture.GetSlot());
        shader.SetUniformMat4fv("u_MVP", mvp);


        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui::StyleColorsDark();

        // ��Ҫָ��glsl�汾, Ҳ����shader�е�version
        const char* glsl_version = "#version 330";
        ImGui_ImplOpenGL3_Init(glsl_version);

        /* ѭ��ֱ���رմ��� */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 view = glm::lookAt(eye, center, up);
            glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 600.0f, -1000.0f, 1000.0f);

            glm::mat4 mvp = proj * view * model;

            // ֻ��Ҫ���°� shader, �������� �� ��������
            shader.Bind();
            shader.SetUniformMat4fv("u_MVP", mvp);
            va.Bind();
            ib.Bind();
            texture.Bind();
            renderer.Draw(va, ib, shader);


            {
                ImGui::Begin("ImGui");
                ImGui::SliderFloat3("translation", &translation.x, -200.0f, 200.0f);
                ImGui::SliderFloat3("eye", &eye.x, -500.0f, 500.0f);
                ImGui::SliderFloat3("center", &center.x, -500.0f, 500.0f);
                ImGui::SliderFloat3("up", &up.x, -1.0f, 1.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* ����ǰ�󻺳��� */
            glfwSwapBuffers(window);

            /* ������ѯ�¼� */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    // �ͷ��� GLFW ��ص���Դ����ȷ����ȷ�عرպ����� GLFW ���״̬
    glfwTerminate();
    return 0;
}