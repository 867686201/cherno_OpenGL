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
	const glm::mat4& getProjViewMatrix() const override { return m_viewProjMatrix; }

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

	// �ṩĬ��ʵ��, �����ظ�
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
		updateViewProjMatrix(); 
	}

protected:
	void updateViewProjMatrix() { m_viewProjMatrix = m_projMatrix * m_viewMatrix; }
	// virtual void updateProjMatrix() = 0;  // �������������ʵ�ָú���, ������ updateViewProjMatrix, ��������������º���, ������� update, ���ܻᵼ�¾���ͬ��
	void updateProjMatrix()		// ʹ�� ģ�巽��ģʽ, ��updateProjMatrix�ĵ����߼���װ��һ�����ɱ���д�ĳ�Ա�����У�Ȼ���ڸú����е���updateProjMatrix��updateViewProjMatrix��
	{							// updateProjMatrix�����һ����ģ�巽�����õĹ��ӣ�hook����������Ը���������ӣ������ܸı�ģ�巽������
		updateProjMatrixImpl();
		updateViewProjMatrix();
	};
	virtual void updateProjMatrixImpl() = 0;
protected:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projMatrix;
	glm::mat4 m_viewProjMatrix{};

	glm::vec3 m_eye;
	glm::vec3 m_center;
	glm::vec3 m_up;
};