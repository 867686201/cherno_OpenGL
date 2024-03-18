#pragma once

#include "OrthoCamera.h"
#include "PerspectCamera.h"
#include <memory>

enum class CameraType
{
	OrthoCamera,
	PerspectCamera
};

using CameraPtr = std::unique_ptr<Camera>;

class CameraFactory
{
public:
	static CameraPtr createOrthoCamera(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return std::make_unique<OrthoCamera>(left, right, bottom, top, zNear, zFar);
	}
	static CameraPtr createPerspectCamera(float fov, float aspect, float zNear, float zFar)
	{
		return std::make_unique<PerspectCamera>(fov, aspect, zNear, zFar);
	}
};