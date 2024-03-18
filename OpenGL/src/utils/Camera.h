#pragma once
#include <glm/glm.hpp>

struct ViewAttrib
{
	glm::vec3 eye{ 0.0f, 0.0f, 0.0f };
	glm::vec3 center{ 0.0f, 0.0f, -1.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	bool isValid() const
	{
		return eye != center && up.length() != 0;
	}
};

struct OrthoParams
{
	float left{ -400.0f };
	float right{ 400.0f };
	float bottom{ -400.0f };
	float top{ 400.0f };
	float zNear{ 0.0f };
	float zFar{ 20000.0f };
	bool isValid() const
	{
		return left != right && top != bottom && zNear != zFar;
	}
	friend bool operator==(const OrthoParams& p1, const OrthoParams& p2)
	{
		return p1.left == p2.left && p1.right == p2.right && p1.bottom == p2.bottom &&
			p1.top == p2.top && p1.zNear == p2.zNear && p1.zFar == p2.zFar;
	}
};

struct PerspectiveParams
{
	float fovy{ 90.0f };
	float aspect{ 1.0f };
	float zNear{ 0 };
	float zFar{ 20000 };
	bool isValid() const
	{
		return fovy != 0.0f && aspect != 0.0f && zNear != zFar;
	}
	friend bool operator==(const PerspectiveParams& p1, const PerspectiveParams& p2)
	{
		return p1.fovy == p2.fovy && p1.aspect == p2.aspect 
			&& p1.zNear == p2.zNear && p1.zFar == p2.zFar;
	}
};

class Camera
{
public:
	virtual ~Camera() = default;

	virtual const glm::mat4& getProjMatrix() const = 0;
	virtual const glm::mat4& getViewMatrix() const = 0;
	virtual const glm::mat4& getProjViewMatrix() const = 0;
	virtual void updateEye(const glm::vec3& eye) = 0;
	virtual void updateCenter(const glm::vec3& center) = 0;
	virtual void updateUp(const glm::vec3& up) = 0;
	virtual void updateLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) = 0;
	virtual void updateOrthoParam(float left, float right, float bottom, float top, float zNear, float zFar) = 0;
	virtual void updatePerspectiveParam(float fov, float aspect, float near, float far) = 0;
};