#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Test.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

namespace test
{
	class TestBatchRender : public Test
	{
	public:
		TestBatchRender();
		~TestBatchRender();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation;
	};
}