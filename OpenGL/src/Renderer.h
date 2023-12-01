#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

class Renderer
{

public:

	void Clear();
	void Clear(float r, float g, float b, float a);
	void Clear(const float color[4]);
	void Clear(glm::vec4& color);
	void Draw(VertexArray& va, IndexBuffer& ib, Shader& shaders);
};