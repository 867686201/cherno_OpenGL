#include "Renderer.h"

#include "GLError.h"

void Renderer::Clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Clear(float r, float g, float b, float a)
{
	GLCall(glClearColor(r, g, b, a));
	Clear();
}

void Renderer::Clear(const float color[4])
{
	Clear(color[0], color[1], color[2], color[3]);
}

void Renderer::Clear(glm::vec4& color)
{
	Clear(color.r, color.g, color.b, color.a);
}

void Renderer::Draw(VertexArray& va, IndexBuffer& ib, Shader& shader)
{
	va.Bind();
	ib.Bind();
	shader.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); 
}
