
#include "TestTinyRenderer.h"

#include "GLError.h"
#include "imgui/imgui.h"

#include <iostream>

namespace test
{
	TestTinyRenderer::TestTinyRenderer()
		: m_Eye(0.0f, 0.0f, 0.0f), m_Center(0.0f, 0.0f, -1.0f), m_Up(0.0f, 1.0f, 0.0f),
		m_camera(CameraFactory::createOrthoCamera(-400.0f * 1.6, 400.0f * 1.6, -400.0f, 400.0f, -1000.0f, 1000.0f)),
		m_tinyRenderer(800, 800)
	{
		m_camera->updateLookAt(m_Eye, m_Center, m_Up);

		float positions[] =
		{
			-400.0f, -400.0f, 0.0f, 0.0f,
			-400.0f,  400.0f, 0.0f, 1.0f,
			 400.0f, -400.0f, 1.0f, 0.0f,
			 400.0f,  400.0f, 1.0f, 1.0f
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


		unsigned char* buffer = GetBuffer();

		m_Shader = std::make_unique<Shader>("OpenGL/res/shaders/basic.shader");
		m_Texture = std::make_unique<Texture>(buffer, m_texWidth, m_texHeight);
		m_Shader->Bind();
		m_Texture->Bind();
		m_Shader->SetUniform1i("u_Texture", m_Texture->GetSlot());

	}
	TestTinyRenderer::~TestTinyRenderer()
	{
	}

	unsigned char* TestTinyRenderer::GetBuffer()
	{
		m_tinyRenderer.addLight(glm::vec4(0, 0, 3, 150));
		m_tinyRenderer.addLight(glm::vec4(5, 5, 1.2, 5000));
		m_tinyRenderer.setViewPos(glm::vec4(5, 5, 1.2, 5000));
		m_tinyRenderer.setModel("OpenGL/res/model/african_head.obj");
		m_tinyRenderer.render();
		m_texWidth = m_tinyRenderer.getWidth();
		m_texHeight = m_tinyRenderer.getHeight();
		return m_tinyRenderer.getFrameBufferData();
	}
	void TestTinyRenderer::OnUpdate(float deltaTime)
	{
	}
	void TestTinyRenderer::OnRender()
	{
		Renderer renderer;
		renderer.Clear(0.0f, 0.0f, 0.0f, 0.0f);

		glm::mat4 mvp = m_camera->getProjViewMatrix();
		m_Shader->SetUniformMat4fv("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);



	}
	void TestTinyRenderer::OnImGuiRender()
	{
		ImGui::Begin("ImGui");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}