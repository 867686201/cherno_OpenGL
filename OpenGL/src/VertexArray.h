#pragma once

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout) const;

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};