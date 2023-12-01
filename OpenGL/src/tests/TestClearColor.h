#pragma once

#include "Test.h"
#include "Renderer.h"

#include "VertexArray.h"

namespace test
{
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];
		Renderer m_Renderer;
	};
}