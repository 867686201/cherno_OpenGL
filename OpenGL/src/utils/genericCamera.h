#pragma once

#include "Camera.h"

#include "utils/Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GenericCamera : public Camera
{
public:
	GenericCamera(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
		: m_eye(eye), m_center(center), m_up(up),
		m_projMatrix(glm::mat4()), m_viewMatrix(utils::lookAt(m_eye, m_center, m_up))
	{
	}
	virtual ~GenericCamera() = default;

	const glm::mat4& getProjMatrix() const override { return m_projMatrix; }
	const glm::mat4& getViewMatrix() const override { return m_viewMatrix; }
	const glm::mat4& getProjViewMatrix() const override { return m_projMatrix * m_viewMatrix; }

	void updateEye(const glm::vec3& eye) override { m_eye = eye; updateViewMatrix(); };
	void updateCenter(const glm::vec3& center) override { m_center = center; updateViewMatrix(); };
	void updateUp(const glm::vec3& up) override { m_up = up; updateViewMatrix(); };
	void updateLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) override
	{
		m_eye = eye;
		m_center = center;
		m_up = up;
		updateViewMatrix();
	}

	// 提供默认实现, 减少重复
	virtual void updateOrthoParam(float left, float right, float bottom, float top, float zNear, float zFar) override
	{

	}
	virtual void updatePerspectiveParam(float fov, float aspect, float near, float far) override
	{

	}

private:
	void updateViewMatrix() 
	{ 
		m_viewMatrix = utils::lookAt(m_eye, m_center, m_up);
		
	}

protected:
	virtual void updateProjMatrix() = 0; 
protected:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projMatrix;

	glm::vec3 m_eye;
	glm::vec3 m_center;
	glm::vec3 m_up;
};