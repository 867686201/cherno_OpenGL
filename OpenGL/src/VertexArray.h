#pragma once

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
public:
	VertexArray(VertexBuffer vb, VertexBufferLayout layout);
	~VertexArray();

	void Bind();
	void Unbind();

private:
	unsigned int m_RendererID;
};