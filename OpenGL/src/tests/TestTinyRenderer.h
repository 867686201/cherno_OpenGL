#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Test.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "utils/CameraFactory.h"

#include "tinyrenderer/TinyRenderer.h"

namespace test
{
	class TestTinyRenderer : public Test
	{
	public:
		TestTinyRenderer();
		~TestTinyRenderer();
		
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		unsigned char* GetBuffer();
		void InitLineAndTri();
		void Draw();
		void DrawLine();
		void ClearColor();
		void UpdateTexture();



		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Camera> m_camera;
		glm::vec3 m_Eye;
		glm::vec3 m_Center;
		glm::vec3 m_Up;

		float m_ClearColor[4];
		int m_texWidth;
		int m_texHeight;
		TinyRenderer m_tinyRenderer;

	};
}