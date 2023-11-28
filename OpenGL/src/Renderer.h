#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

class Renderer
{

public:

	void Clear();
	void Draw(VertexArray& va, IndexBuffer& ib, Shader& shaders);
};