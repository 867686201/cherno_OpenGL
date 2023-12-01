#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>


// GL/glew.h 需要在 GLFW 等头文件之前

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

    /* 初始化库 */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* 创建 window 和它的 OpenGL 上下文 */
    window = glfwCreateWindow(960, 600, "Hello World", NULL, NULL);
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

    //glfwSwapInterval(2); // 每等到 interval 个垂直同步信号后开始渲染新的一帧到后缓冲区, 如果为 0 则是关闭垂直同步, 帧率没有上限
                         // 60hz屏幕下, 1 为 60hz, 2 为 30hz, 3 为 20 hz

    // 必须在初始化上下文之后调用 glewInit
    if (glewInit() != GLEW_OK)
        std::cout << "glewInitError" << std::endl;

    // 当有一个有效的 OpenGL 上下文之后，可以输出版本
    // 4.6.14756 Compatibility Profile Context 20.40.52 27.20.14052.10
    unsigned char* glVersion;  // 提升作用域
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

        // 需要指定glsl版本, 也就是shader中的version
        const char* glsl_version = "#version 330";
        ImGui_ImplOpenGL3_Init(glsl_version);

        /* 循环直到关闭窗口 */
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

            // 只需要重新绑定 shader, 顶点数组 和 索引缓冲
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

            /* 交换前后缓冲区 */
            glfwSwapBuffers(window);

            /* 处理轮询事件 */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    // 释放与 GLFW 相关的资源，并确保正确地关闭和清理 GLFW 库的状态
    glfwTerminate();
    return 0;
}