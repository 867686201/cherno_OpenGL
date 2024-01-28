
#include "TestBatchRender.h"

#include "GLError.h"
#include "imgui/imgui.h"

namespace test
{
	TestBatchRender::TestBatchRender()
		: m_Proj(glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Translation(glm::vec3(0, 0, 0))
	{


		float vertices[] = {
			// vec2 λ��     vec4 ��ɫ                 vec2 uv���� vec1 ������
			100.0f, 100.0f,	0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,	    // ������1
			200.0f, 100.0f,	0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
			200.0f, 200.0f,	0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
			100.0f, 200.0f,	0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

			300.0f, 100.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,		// ������2
			400.0f, 100.0f,	1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,
			400.0f, 200.0f,	1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,
			300.0f, 200.0f,	1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,

		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VBO = std::make_unique<VertexBuffer>(vertices, 9 * 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);  // λ��
		layout.Push<float>(4);  // ��ɫ
		layout.Push<float>(2);	// uv����
		layout.Push<float>(1);	// ������
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indices, 12);

		m_Shader = std::make_unique<Shader>("OpenGL/res/shaders/batch.shader");
		m_Shader->Bind();
		m_Texture[0] = std::make_unique<Texture>("OpenGL/res/textures/test.png");
		m_Texture[1] = std::make_unique<Texture>("OpenGL/res/textures/ChernoLogo.png");
		for (size_t i = 0; i < 2; i++)
		{
			m_Texture[i]->Bind(i);
		}
		int samplers[2] = { 0, 1 };
		m_Shader->SetUniform1iv("u_Textures", 2, samplers);

	}
	TestBatchRender::~TestBatchRender()
	{
	}
	void TestBatchRender::OnUpdate(float deltaTime)
	{
	}
	void TestBatchRender::OnRender()
	{
		Renderer renderer;
		renderer.Clear(0.0f, 0.0f, 0.0f, 0.0f);

		glm::mat4 mvp = m_Proj * m_View;
		m_Shader->SetUniformMat4fv("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);


	}
	void TestBatchRender::OnImGuiRender()
	{
		ImGui::Begin("ImGui");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}