#pragma once

#include <vector>

#include "GL/glew.h"

struct VertexLayoutElement
{
	// 对应 glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	// 其中的 GLint size, GLenum type, GLboolean normalized 
	// GLboolean 的实际类型为 unsigned char, GLsizei 为 int 
	unsigned int count;  
	unsigned int type;
	unsigned char normalized; 
	int GetLength()
	{
		int length = 0;
		switch (type)
		{
			case GL_FLOAT: length = 4 * count; break;
			case GL_UNSIGNED_INT: length = 4 * count; break;
			case GL_UNSIGNED_BYTE: length = 1 * count; break;
			default: break;
		}
		return length;
	}
};

class VertexBufferLayout
{

public:
	VertexBufferLayout() : m_Stride(0), m_Count(0) {};
	~VertexBufferLayout() {};

	template<typename T>
	void Push(int count)
	{
		static_assert(false);
	}
	template<>
	void Push<float>(int count)
	{
		VertexLayoutElement element{ (unsigned int)count, GL_FLOAT, GL_FALSE };
		m_Elements.emplace_back(element);
		m_Offset.emplace_back(m_Stride);
		m_Stride += element.GetLength();
	}

	inline VertexLayoutElement GetElement(unsigned int index) const { return m_Elements[index]; }
	inline std::vector<VertexLayoutElement> GetElements() const { return m_Elements; }
	inline int GetStride() const { return m_Stride; }
	inline int GetOffset(int i) const { return m_Offset[i]; }

private:
	std::vector<VertexLayoutElement> m_Elements;
	std::vector<unsigned int> m_Offset;
	int m_Stride;
};