#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray(VertexBuffer vb, VertexBufferLayout layout)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
	vb.Bind();
	for (int i = 0; i < layout.GetCount(); i++)
	{
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, layout.GetElement(i).count, layout.GetElement(i).type, layout.GetElement(i).normalized,
			layout.GetStride(), (void*)layout.GetOffset(i)));
	}

}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind()
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind()
{
	GLCall(glBindVertexArray(0));
}
