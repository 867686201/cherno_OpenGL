#pragma once

#include "Test.h"
#include "Renderer.h"


namespace test
{
	class TestTransform : public Test
	{
	public:
		TestTransform();
		~TestTransform();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Renderer m_Renderer;
		std::unique_ptr<VertexArray> m_VAO1;
		std::unique_ptr<IndexBuffer> m_IndexBuffer1;
		std::unique_ptr<VertexBuffer> m_VertexBuffer1;

		std::unique_ptr<VertexArray> m_VAO2;
		std::unique_ptr<IndexBuffer> m_IndexBuffer2;
		std::unique_ptr<VertexBuffer> m_VertexBuffer2;
		std::unique_ptr<Shader> m_Shader;

		// 摄像机位置
		glm::vec3 m_Eye;
		// 目标位置
		glm::vec3 m_Center;
		// 上向量
		glm::vec3 m_Up;
	};
}