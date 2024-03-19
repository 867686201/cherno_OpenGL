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
	static CameraPtr createOrthoCamera(float left = -400 * 1.6, float right = 400 * 1.6, float bottom = -400, float top = 400, float zNear = -1000, float zFar = 1000)
	{
		return std::make_unique<OrthoCamera>(left, right, bottom, top, zNear, zFar);
	}
	static CameraPtr createPerspectCamera(float fov = 60, float aspect = 1, float zNear = -1000, float zFar = 1000)
	{
		return std::make_unique<PerspectCamera>(fov, aspect, zNear, zFar);
	}

};