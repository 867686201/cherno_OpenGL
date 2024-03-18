#pragma once

#include "GenericCamera.h"
#include "glm/gtc/matrix_transform.hpp"

class PerspectCamera : public GenericCamera
{
public:
	PerspectCamera(float fov, float aspect, float zNear, float zFar)
		: GenericCamera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)),
		m_fov(fov), m_aspect(aspect), m_zNear(zNear), m_zFar(zFar)
	{
		updateProjMatrix();
	}

	void updateProjMatrixImpl() override
	{
		m_projMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar); // 角度转为弧度
	}

	void updatePerspectiveParam(float fov, float aspect, float near, float far) override
	{
		m_fov = fov;
		m_aspect = aspect;
		m_zNear = near;
		m_zFar = far;
		updateProjMatrix();
	}

private:
	float m_fov;
	float m_aspect;
	float m_zNear;
	float m_zFar;
};