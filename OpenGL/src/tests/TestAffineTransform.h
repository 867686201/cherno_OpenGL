#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Test.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "utils/CameraFactory.h"

namespace test
{
	class TestAffineTransform : public Test
	{
	public:
		TestAffineTransform();
		~TestAffineTransform();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:


		void TranslationGUI();
		void ScaleGUI();
		void RotateGUI();
		void AnyAxisRotationGUI();

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;		
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Camera> m_Camera;

		std::unique_ptr<VertexArray> m_AxisVAO;
		std::unique_ptr<VertexBuffer> m_AxisVBO;
		std::unique_ptr<IndexBuffer> m_AxisIBO;
		std::unique_ptr<Shader> m_AxisShader;


		glm::vec3 m_Eye;
		glm::vec3 m_Center;
		glm::vec3 m_Up;
		glm::mat4 m_Model;
		glm::vec3 m_Translation{ -100.0f, 0.0f, 0.0f };
		float m_Angle{ 0.0f };
		glm::vec3 m_Scale{ 1.0f, 1.0f, 1.0f };
		glm::vec4 m_AnyAxisRotation{ 60.0f, 1.0f, 1.0f, 1.0f };


	};
}