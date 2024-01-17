
#include "TestTransform.h"

#include "GLError.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "myHelpFunction/generate.h"

namespace test
{
	TestTransform::TestTransform()
		: m_Eye(1.0f, 1.0f, 1.0f), m_Center(0.0f, 0.0f, 0.0f), m_Up(0.0f, 1.0f, 0.0f) 
	{
		float m_AxisVertices[] = 
		{
			0.0f, 0.0f, 0.0f,  // 原点
			0.7f, 0.0f, 0.0f,  // X 轴端点
			0.0f, 0.7f, 0.0f,  // Y 轴端点
			0.0f, 0.0f, 0.7f   // Z 轴端点
		};
		unsigned int m_AxisIndices[] = { 0, 1, 0, 2, 0, 3 };
		m_VAO1 = std::make_unique<VertexArray>();
		m_VertexBuffer1 = std::make_unique<VertexBuffer>(m_AxisVertices, 4 * 3 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		m_VAO1->AddBuffer(*m_VertexBuffer1, layout);
		m_IndexBuffer1 = std::make_unique<IndexBuffer>(m_AxisIndices, 6);

		std::vector<float> sphereVertices = generateSphereVertices(0.0, 0.0, 0.1, 0.05, 20, 20);
		std::vector<unsigned int> sphereIndices = generateSphereIndices(20, 20);
		m_VAO2 = std::make_unique<VertexArray>();
		m_VertexBuffer2 = std::make_unique<VertexBuffer>(sphereVertices.data(), sphereVertices.size() * sizeof(float));
		VertexBufferLayout layout2;
		layout2.Push<float>(3);
		m_VAO2->AddBuffer(*m_VertexBuffer2, layout2);
		m_IndexBuffer2 = std::make_unique<IndexBuffer>(sphereIndices.data(), sphereIndices.size());


		glm::mat4 orthoMatrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
		glm::mat4 view = glm::lookAt(m_Eye, m_Center, m_Up);
		glm::mat4 mvp = orthoMatrix * view;



		m_Shader = std::make_unique<Shader>("OpenGL/res/shaders/axis.shader");
		m_Shader->Bind();
		m_Shader->SetUniformMat4fv("u_MVP", mvp);
	}
	TestTransform::~TestTransform()
	{
	}
	void TestTransform::OnUpdate(float deltaTime)
	{
	}
	void TestTransform::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		{
			m_Shader->Bind();

			glm::mat4 orthoMatrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
			glm::mat4 view = glm::lookAt(m_Eye, m_Center, m_Up);
			glm::mat4 mvp = orthoMatrix * view;
			m_Shader->SetUniformMat4fv("u_MVP", mvp);
			renderer.DrawLine(*m_VAO1, *m_IndexBuffer1, *m_Shader);
			renderer.Draw(*m_VAO2, *m_IndexBuffer2, *m_Shader);
		}


	}
	void TestTransform::OnImGuiRender()
	{
		ImGui::SliderFloat3("eye", &m_Eye.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("center", &m_Center.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("up", &m_Up.x, -1.0f, 1.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}