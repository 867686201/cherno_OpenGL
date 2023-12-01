
#include "TestClearColor.h"

#include "GLError.h"
#include "imgui/imgui.h"

namespace test
{
	TestClearColor::TestClearColor()
		: m_ClearColor{ 0.4f, 0.2f, 0.5f, 1.0f }
	{
	}
	TestClearColor::~TestClearColor()
	{
	}
	void TestClearColor::OnUpdate(float deltaTime)
	{
	}
	void TestClearColor::OnRender()
	{
		m_Renderer.Clear(m_ClearColor);
	}
	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("ClearColor", m_ClearColor);
	}
}