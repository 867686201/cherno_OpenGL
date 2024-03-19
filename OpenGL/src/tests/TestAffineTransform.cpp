
#include "TestAffineTransform.h"
#include "utils/Transform.h"
#include "GLError.h"
#include "imgui/imgui.h"

#include <iomanip> // 添加这个头文件以使用 std::fixed 和 std::setprecision
#include <glm/gtc/epsilon.hpp> // 包含epsilonEqual函数

#include <iostream>

namespace test
{
	TestAffineTransform::TestAffineTransform()
		: 
		m_Eye(800.0f, 800.0f, 800.0f), m_Center(0.0f, 0.0f, 0.0f), m_Up(0.0f, 1.0f, 0.0f),
		m_Camera(CameraFactory::createPerspectCamera()),
		m_Model(glm::translate(glm::mat4(1.0f), m_Translation))
	{
		m_Camera->updateLookAt(m_Eye, m_Center, m_Up);

		float positions[] = {
			// 前面
			-200.0f, -100.0f,  100.0f,  0.0f, 0.0f, // 左下
			 200.0f, -100.0f,  100.0f,  1.0f, 0.0f, // 右下
			 200.0f,  100.0f,  100.0f,  1.0f, 1.0f, // 右上
			-200.0f,  100.0f,  100.0f,  0.0f, 1.0f, // 左上
			// 后面
			-200.0f, -100.0f, -100.0f,  1.0f, 0.0f,
			-200.0f,  100.0f, -100.0f,  1.0f, 1.0f,
			 200.0f,  100.0f, -100.0f,  0.0f, 1.0f,
			 200.0f, -100.0f, -100.0f,  0.0f, 0.0f,
			 // 左面
			-200.0f, -100.0f, -100.0f,  0.0f, 0.0f,
			-200.0f, -100.0f,  100.0f,  1.0f, 0.0f,
			-200.0f,  100.0f,  100.0f,  1.0f, 1.0f,
			-200.0f,  100.0f, -100.0f,  0.0f, 1.0f,
			// 右面
			 200.0f, -100.0f, -100.0f,  1.0f, 0.0f,
			 200.0f,  100.0f, -100.0f,  1.0f, 1.0f,
			 200.0f,  100.0f,  100.0f,  0.0f, 1.0f,
			 200.0f, -100.0f,  100.0f,  0.0f, 0.0f,
			// 上面
			-200.0f,  100.0f, -100.0f,  0.0f, 1.0f,
			-200.0f,  100.0f,  100.0f,  0.0f, 0.0f,
			 200.0f,  100.0f,  100.0f,  1.0f, 0.0f,
			 200.0f,  100.0f, -100.0f,  1.0f, 1.0f,
			// 下面
			-200.0f, -100.0f, -100.0f,  1.0f, 1.0f,
			 200.0f, -100.0f, -100.0f,  0.0f, 1.0f,
			 200.0f, -100.0f,  100.0f,  0.0f, 0.0f,
			-200.0f, -100.0f,  100.0f,  1.0f, 0.0f,
		};



		unsigned int indices[] = {
			// 前面
			0, 1, 2,  0, 2, 3,
			// 后面
			4, 5, 6,  4, 6, 7,
			// 左面
			8, 9, 10,  8, 10, 11,
			// 右面
			12, 13, 14,  12, 14, 15,
			// 上面
			16, 17, 18,  16, 18, 19,
			// 下面
			20, 21, 22,  20, 22, 23,
		};


		m_VBO = std::make_unique<VertexBuffer>(positions, 5 * 4 * 6 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indices, 6 * 6);

		m_Shader = std::make_unique<Shader>("OpenGL/res/shaders/basic.shader");
		m_Texture = std::make_unique<Texture>("OpenGL/res/textures/test.png");
		m_Shader->Bind();
		m_Texture->Bind();
		m_Shader->SetUniform1i("u_Texture", m_Texture->GetSlot());

		float axisVertices[] = {
			// X轴，红色
			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			400.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			// Y轴，绿色
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 400.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			// Z轴，蓝色
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 400.0f, 0.0f, 0.0f, 1.0f
		};
		m_AxisVBO = std::make_unique<VertexBuffer>(axisVertices, 6 * 6 * sizeof(float));
		VertexBufferLayout axisLayout;
		axisLayout.Push<float>(3);
		axisLayout.Push<float>(3);
		m_AxisVAO = std::make_unique<VertexArray>();
		m_AxisVAO->AddBuffer(*m_AxisVBO, axisLayout);

		m_AxisShader = std::make_unique<Shader>("OpenGL/res/shaders/axis.shader");
		m_AxisShader->Bind();

	}
	TestAffineTransform::~TestAffineTransform()
	{
	}
	void TestAffineTransform::OnUpdate(float deltaTime)
	{
	}
	void TestAffineTransform::OnRender()
	{
		Renderer renderer;
		renderer.Clear(0.0f, 0.0f, 0.0f, 0.0f);

		m_Camera->updateLookAt(m_Eye, m_Center, m_Up);
		glm::mat4 mvp = m_Camera->getProjViewMatrix() * m_Model;
		m_Shader->SetUniformMat4fv("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

		m_AxisShader->SetUniformMat4fv("u_MVP", mvp);
		renderer.DrawLines(*m_AxisVAO, 3, *m_AxisShader);


	}
	void TestAffineTransform::OnImGuiRender()
	{
		ImGui::Begin("ImGui");
		ImGui::Text("view transform");
		ImGui::SliderFloat3("eye", &m_Eye.x, -500.0f, 1000.0f);
		ImGui::SliderFloat3("center", &m_Center.x, -500.0f, 500.0f);
		ImGui::SliderFloat3("up", &m_Up.x, -1.0f, 1.0f);


		ImGui::Text("affine transform");
		TranslationGUI();
		ScaleGUI();
		RotateGUI();
		AnyAxisRotationGUI();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	void TestAffineTransform::TranslationGUI()
	{
		ImGui::InputFloat3("translation", &m_Translation.x);
		if (ImGui::Button("translation"))
		{
			m_Model = translation(m_Translation.x, m_Translation.y, m_Translation.z) * m_Model;
		}
	}
	void TestAffineTransform::ScaleGUI()
	{
		ImGui::InputFloat3("scale", &m_Scale.x);
		if (ImGui::Button("scale"))
		{
			m_Model = scale(m_Scale.x, m_Scale.y, m_Scale.z) * m_Model;

		}
	}
	void TestAffineTransform::RotateGUI()
	{

		const char* items[] = { "X", "Y", "Z" };
		Axis axis = Axis::m_x_axis;
		static int item_current = 0; // 当前选择的轴，默认是X轴
		if (ImGui::BeginCombo("Axis", items[item_current]))  // Combo 下拉菜单
		{
			for (int n = 0; n < 3; n++)
			{
				bool is_selected = (item_current == n); // 检查当前项是否被选中
				if (ImGui::Selectable(items[n], is_selected))
					item_current = n;

				// 将当前选中的项设置为列表框的标题
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}


		ImGui::InputFloat("rotateAngle", &m_Angle);
		if (ImGui::Button("rotate"))
		{
			
			switch (item_current)
			{
				case 0: axis = Axis::m_x_axis; break; 
				case 1: axis = Axis::m_y_axis; break; 
				case 2: axis = Axis::m_z_axis; break; 
			}

		
			m_Model = rotate(Dimension::dim_3D, axis, m_Angle) * m_Model;
		}
	}
	void TestAffineTransform::AnyAxisRotationGUI()
	{
		ImGui::InputFloat4("any axis rotate", &m_AnyAxisRotation.x);
		if (ImGui::Button("any_rotate"))
		{
			m_Model = arbitrary_3D_rotation(m_AnyAxisRotation.x, m_AnyAxisRotation.y, m_AnyAxisRotation.z, m_AnyAxisRotation.w) * m_Model;

		}
	}
}