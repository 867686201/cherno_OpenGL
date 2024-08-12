#include "TinyRenderer.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

TinyRenderer::TinyRenderer() : m_width(800), m_height(800), m_frameBuffer(nullptr) {}


TinyRenderer::TinyRenderer(int width = 800, int height = 800)
	: m_width(width), m_height(height), m_frameBuffer(std::make_unique<TGAImage>(width, height, TGAImage::RGBA)), 
	m_zbuffer(width*height, std::numeric_limits<double>::lowest())
{
}

void TinyRenderer::render()
{
	if (!m_model || !m_frameBuffer) return;
	for (int i = 0; i < m_model->nfaces(); i++)
	{
		Face face = m_model->face(i);
		std::vector<glm::vec3> screen_coords;
		std::vector<glm::vec3> world_coords;
		for (int j = 0; j < 3; j++)
		{
			glm::vec3 vert = m_model->vert(face[j]);
			world_coords.push_back(vert);
			screen_coords.push_back(worldToScreen(vert));
		}

		glm::vec3 n = glm::normalize(glm::cross(world_coords[1] - world_coords[0], world_coords[2] - world_coords[0]));	// 计算三角形法向量

		if (glm::dot(m_viewPos, n) < 0) continue;		// 背面剔除, 如果是背面, 则下一个三角形, 不进行光照计算和光栅化

		glm::vec3 result_color = calculateLighting(world_coords[0], n); // 平面着色, 取的三角形第一个顶点
		triangle(screen_coords, world_coords, m_zbuffer, *m_frameBuffer, result_color);	// 三角形光栅化
	}
}

void TinyRenderer::setModel(const std::string& filePath)
{
	
	m_model = std::make_shared<Model>(filePath.c_str());
}




glm::vec3 TinyRenderer::calculateLighting(const glm::vec3& point, const glm::vec3& normal) const
{
	float ka = 0.5;
	float kd = 0.5;
	float ks = 0.8;
	int p = 15;

	glm::vec3 result_color(0.0f, 0.0f, 0.0f);
	glm::vec3 ambient_intensity = glm::vec3(100, 100, 100);

	auto ambientCal = [&]()		// 环境光 ka * Ia
		{
			return ka * ambient_intensity;
		};
	result_color += ambientCal();
	for (auto light : m_lights)
	{
		glm::vec3 lightPos = glm::vec3(light);
		glm::vec3 light_intensity = glm::vec3(light.a, light.a, light.a);
		float r = glm::length(lightPos - point);			// 光线距离
		glm::vec3 l = glm::normalize(lightPos - point);
		glm::vec3 v = glm::normalize(m_viewPos - point);
		glm::vec3 half_vec = glm::normalize(l + v);			// 半程向量
		auto diffuseCal = [&]()		// 漫反射 kd * (I/r^2) * max(0, n*l)
			{
				float n_dot_l = std::max(0.0f, glm::dot(normal, l));
				return kd * (light_intensity / (r * r)) * n_dot_l;
			};
		auto specularCal = [&]()		// 高光项 ks * (I/r^2) * max(0, n*h)^p
			{
				float n_dot_h = std::pow(std::max(0.0f, glm::dot(normal, half_vec)), p);
				return ks * (light_intensity / (r * r)) * n_dot_h;
			};
		result_color += diffuseCal();
		result_color += specularCal();
	}
	return result_color;
}
