#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout) const
{
	Bind();
	vb.Bind();
	for (int i = 0; i < layout.GetCount(); i++)
	{
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, layout.GetElement(i).count, layout.GetElement(i).type, layout.GetElement(i).normalized,
			layout.GetStride(), (void*)layout.GetOffset(i)));
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
