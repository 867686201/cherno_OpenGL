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
	class TestTexture3D : public Test
	{
	public:
		TestTexture3D();
		~TestTexture3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<GenericCamera> m_Camera;


		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;
		glm::vec3 m_Eye;
		glm::vec3 m_Center;
		glm::vec3 m_Up;
		glm::mat4 m_Model;

		float m_Fov;
		float m_Aspect;
		float m_Near;
		float m_Far;


	};
}