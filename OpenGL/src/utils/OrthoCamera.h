#pragma once

#include "GenericCamera.h"
#include "glm/gtc/matrix_transform.hpp"

class OrthoCamera : public GenericCamera
{
public:
	OrthoCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f)
		: GenericCamera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)),
		m_left(left), m_right(right), m_bottom(bottom), m_top(top),
		m_zNear(zNear), m_zFar(zFar)
	{
		
		updateProjMatrix();
	}

	void updateOrthoParam(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f) override
	{
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
		m_zNear = zNear;
		m_zFar = zFar;
		updateProjMatrix();
	}

private:
	void updateProjMatrix() override
	{
		m_projMatrix = glm::ortho(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
	}

	float m_left;
	float m_right;
	float m_bottom;
	float m_top;
	float m_zNear;
	float m_zFar;
};