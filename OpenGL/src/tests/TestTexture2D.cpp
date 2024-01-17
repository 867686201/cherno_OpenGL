
#include "TestTexture2D.h"

#include "GLError.h"
#include "imgui/imgui.h"

namespace test
{
	TestTexture2D::TestTexture2D()
		: m_TranslationA(100.0f, 100.0f, 0.0f), m_TranslationB(200.0f, 100.0f, 0.0f),
		m_Eye(0.0f, 0.0f, 400.0f), m_Center(0.0f, 0.0f, 0.0f), m_Up(0.0f, 1.0f, 0.0f),
		m_Model(glm::translate(glm::mat4(1.0f), m_TranslationA)),
		m_View(glm::lookAt(m_Eye, m_Center, m_Up)), 
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 600.0f, -1000.0f, 1000.0f))
	{


		float positions[] =
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

		m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("OpenGL/res/shaders/basic.shader");
		m_Texture = std::make_unique<Texture>("OpenGL/res/textures/ChernoLogo.png");
		m_Shader->Bind();
		m_Texture->Bind();
		m_Shader->SetUniform1i("u_Texture", m_Texture->GetSlot());

	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}
	void TestTexture2D::OnRender()
	{
		Renderer renderer;
		renderer.Clear(0.0f, 0.0f, 0.0f, 0.0f);

		m_Model = glm::translate(glm::mat4(1.0f), m_TranslationA);
		m_View = glm::lookAt(m_Eye, m_Center, m_Up);
		glm::mat4 mvp = m_Proj * m_View * m_Model;
		m_Shader->SetUniformMat4fv("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

		m_Model = glm::translate(glm::mat4(1.0f), m_TranslationB);
		mvp = m_Proj * m_View * m_Model;
		m_Shader->SetUniformMat4fv("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);


	}
	void TestTexture2D::OnImGuiRender()
	{
		ImGui::Begin("ImGui");
		ImGui::SliderFloat3("translationA", &m_TranslationA.x, -200.0f, 800.0f);
		ImGui::SliderFloat3("eye", &m_Eye.x, -500.0f, 500.0f);
		ImGui::SliderFloat3("center", &m_Center.x, -500.0f, 500.0f);
		ImGui::SliderFloat3("up", &m_Up.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("translationB", &m_TranslationB.x, -200.0f, 800.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}