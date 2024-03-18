
#include "TestTexture3D.h"

#include "GLError.h"
#include "imgui/imgui.h"

#include <iostream>

namespace test
{
	TestTexture3D::TestTexture3D()
		: m_TranslationA(-100.0f, 0.0f, 0.0f), m_TranslationB( 100.0f, 0.0f, 0.0f),
		m_Eye(0.0f, 400.0f, 800.0f), m_Center(0.0f, 0.0f, 0.0f), m_Up(0.0f, 1.0f, 0.0f),
		m_Fov(60.0), m_Aspect(1), m_Near(-100), m_Far(300),
		m_Camera(CameraFactory::createPerspectCamera(m_Fov, m_Aspect, m_Near, m_Far)),
		m_Model(glm::translate(glm::mat4(1.0f), m_TranslationA))
	{
		m_Camera->updateLookAt(m_Eye, m_Center, m_Up);

		float positions[] = {
			// 前面
			-100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
			 100.0f, -100.0f,  100.0f,  1.0f, 0.0f,
			 100.0f,  100.0f,  100.0f,  1.0f, 1.0f,
			-100.0f,  100.0f,  100.0f,  0.0f, 1.0f,
			// 后面
			-100.0f, -100.0f, -100.0f,  1.0f, 0.0f,
			-100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
			 100.0f,  100.0f, -100.0f,  0.0f, 1.0f,
			 100.0f, -100.0f, -100.0f,  0.0f, 0.0f,
			// 左面
			-100.0f, -100.0f, -100.0f,  0.0f, 0.0f,
			-100.0f, -100.0f,  100.0f,  1.0f, 0.0f,
			-100.0f,  100.0f,  100.0f,  1.0f, 1.0f,
			-100.0f,  100.0f, -100.0f,  0.0f, 1.0f,
			// 右面
			 100.0f, -100.0f, -100.0f,  1.0f, 0.0f,
			 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
			 100.0f,  100.0f,  100.0f,  0.0f, 1.0f,
			 100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
			// 上面
			-100.0f,  100.0f, -100.0f,  0.0f, 1.0f,
			-100.0f,  100.0f,  100.0f,  0.0f, 0.0f,
			 100.0f,  100.0f,  100.0f,  1.0f, 0.0f,
			 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
			// 下面
			-100.0f, -100.0f, -100.0f,  1.0f, 1.0f,
			 100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
			 100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
			-100.0f, -100.0f,  100.0f,  1.0f, 0.0f,
		};


		unsigned int indices[] = {
			// 前面
			0, 1, 2,  0, 2, 3,
			// 后面
			4, 5, 6,  4, 6, 7,
			// 左面
			8, 9, 10,  8, 10, 11,
			// 右面
			12, 13, 14,  12, 14, 15,
			// 上面
			16, 17, 18,  16, 18, 19,
			// 下面
			20, 21, 22,  20, 22, 23,
		};


		m_VBO = std::make_unique<VertexBuffer>(positions, 5 * 4 * 6 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indices, 6 * 6);

		m_Shader = std::make_unique<Shader>("OpenGL/res/shaders/basic.shader");
		m_Texture = std::make_unique<Texture>("OpenGL/res/textures/test.png");
		m_Shader->Bind();
		m_Texture->Bind();
		m_Shader->SetUniform1i("u_Texture", m_Texture->GetSlot());

	}
	TestTexture3D::~TestTexture3D()
	{
	}
	void TestTexture3D::OnUpdate(float deltaTime)
	{
	}
	void TestTexture3D::OnRender()
	{
		Renderer renderer;
		renderer.Clear(0.0f, 0.0f, 0.0f, 0.0f);

		m_Model = glm::translate(glm::mat4(1.0f), m_TranslationA);
		m_Camera->updateLookAt(m_Eye, m_Center, m_Up);
		m_Camera->updatePerspectiveParam(m_Fov, m_Aspect, m_Near, m_Far);
		glm::mat4 mvp = m_Camera->getProjViewMatrix() * m_Model;
		m_Shader->SetUniformMat4fv("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

		m_Model = glm::translate(glm::mat4(1.0f), m_TranslationB);
		mvp = m_Camera->getProjViewMatrix() * m_Model;
		m_Shader->SetUniformMat4fv("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);


	}
	void TestTexture3D::OnImGuiRender()
	{
		ImGui::Begin("ImGui");
		ImGui::Text("model transform");
		ImGui::SliderFloat3("translationA", &m_TranslationA.x, - 100.0f, 800.0f);
		ImGui::SliderFloat3("translationB", &m_TranslationB.x, - 100.0f, 800.0f);
		ImGui::Text("view transform");
		ImGui::SliderFloat3("eye", &m_Eye.x, -500.0f, 500.0f);
		ImGui::SliderFloat3("center", &m_Center.x, -500.0f, 500.0f);
		ImGui::SliderFloat3("up", &m_Up.x, -1.0f, 1.0f);
		ImGui::Text("proj transform");
		ImGui::SliderFloat("fov", &m_Fov, 30.0f, 75.0f);
		ImGui::SliderFloat("aspect", &m_Aspect, 0.5f, 1.5f);
		ImGui::SliderFloat("near", &m_Near, -100.0f, 500.0f);
		ImGui::SliderFloat("far", &m_Far, -100.0f, 500.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}