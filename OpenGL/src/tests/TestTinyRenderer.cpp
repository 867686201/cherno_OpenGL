
#include "TestTinyRenderer.h"

#include "GLError.h"
#include "imgui/imgui.h"

#include <iostream>

namespace test
{
	TestTinyRenderer::TestTinyRenderer()
		: m_Eye(0.0f, 0.0f, 0.0f), m_Center(0.0f, 0.0f, -1.0f), m_Up(0.0f, 1.0f, 0.0f),
		m_camera(CameraFactory::createOrthoCamera(-400.0f * 1.6, 400.0f * 1.6, -400.0f, 400.0f, -1000.0f, 1000.0f)),
		m_tinyRenderer(800, 800), m_ClearColor{ 0.4f, 0.2f, 0.5f, 1.0f }
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
		InitLineAndTri();
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

		m_texWidth = m_tinyRenderer.getWidth();
		m_texHeight = m_tinyRenderer.getHeight();
		return m_tinyRenderer.getFrameBufferData();
	}
	void TestTinyRenderer::InitLineAndTri()
	{
		m_tinyRenderer.addLight(glm::vec4(0, 0, 3, 150));
		m_tinyRenderer.addLight(glm::vec4(5, 5, 1.2, 5000));
		m_tinyRenderer.setViewPos(glm::vec3(0, 0, 3));
		m_tinyRenderer.setModel("OpenGL/res/model/african_head.obj");

		const TGAColor white = TGAColor(255, 255, 255, 255);
		const TGAColor red = TGAColor(255, 0, 0, 255);
		const TGAColor blue = TGAColor(0, 0, 255, 255);
		const TGAColor black = TGAColor(0, 0, 0, 255);
		m_tinyRenderer.addLine(glm::vec2(-500, -800), glm::vec2(500, 800), red);
		m_tinyRenderer.addLine(glm::vec2(-400, -200), glm::vec2(400, 200), blue);
		m_tinyRenderer.addLine(glm::vec2(-100, -300), glm::vec2(100, 300), black);
	}
	void TestTinyRenderer::Draw()
	{
		
		m_tinyRenderer.render();
	}
	void TestTinyRenderer::DrawLine()
	{
		m_tinyRenderer.renderLine();
	}
	void TestTinyRenderer::ClearColor()
	{
		m_tinyRenderer.clearFrameBuffer(glm::vec4(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
	}
	void TestTinyRenderer::UpdateTexture()
	{
		if (!m_Texture) {
			std::cerr << "Error: m_Texture is nullptr in UpdateTexture!" << std::endl;
			return;
		}
		unsigned char* buffer = GetBuffer();
		m_Texture->UpdateTexture(buffer);

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
		m_Texture->Bind();
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);



	}
	void TestTinyRenderer::OnImGuiRender()
	{
		ImGui::Begin("ImGui");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		// 添加按钮以选择绘制线条或三角形
		if (ImGui::Button("Draw Line")) 
		{
			DrawLine();
			UpdateTexture();
		}
		if (ImGui::Button("Draw Triangle")) 
		{
			Draw();
			UpdateTexture();
		}
		ImGui::ColorEdit4("ClearColor", m_ClearColor);
		if (ImGui::Button("Clear"))
		{
			ClearColor();
			UpdateTexture();
		}

		ImGui::End();
	}

}