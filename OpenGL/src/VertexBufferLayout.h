#pragma once

#include <vector>

#include "GL/glew.h"
#include "GLError.h"

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
		switch (type)
		{
			case GL_FLOAT: return 4 * count; 
			case GL_UNSIGNED_INT: return 4 * count; 
			case GL_UNSIGNED_BYTE: return 1 * count; 
		}
		ASSERT(false);  // 如果没有对应的 type 则终止程序
		return 0;
	}
	static unsigned int GetSizeOfType(unsigned int type) // 视频版本 static 函数不能使用结构体中的变量
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{

public:
	VertexBufferLayout() : m_Stride(0) {};
	~VertexBufferLayout() {};

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);  // 在编译时, 如果输入了没有在下方特例化的类型, 则会报错
	}
	template<>
	void Push<float>(unsigned int count)
	{
		VertexLayoutElement element{ count, GL_FLOAT, GL_FALSE };
		m_Elements.emplace_back(element);
		m_Offset.emplace_back(m_Stride);
		m_Stride += element.GetLength();
		//m_Stride += count * VertexLayoutElement::GetSizeOfType(GL_FLOAT);
	}
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		VertexLayoutElement element{ count, GL_UNSIGNED_INT, GL_FALSE };
		m_Elements.emplace_back(element);
		m_Offset.emplace_back(m_Stride);
		m_Stride += element.GetLength();
		//m_Stride += count * VertexLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
	}
	template<>
	void Push<unsigned char>(unsigned int count)
	{
		VertexLayoutElement element{ count, GL_UNSIGNED_BYTE, GL_TRUE };
		m_Elements.emplace_back(element);
		m_Offset.emplace_back(m_Stride);
		m_Stride += element.GetLength();
		//m_Stride += count * VertexLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline VertexLayoutElement GetElement(unsigned int index) const { return m_Elements[index]; }
	inline const std::vector<VertexLayoutElement> GetElements() const { return m_Elements; } // m_Elements 返回后不想要被修改, 在前方加入 const
	inline unsigned int GetStride() const { return m_Stride; }
	inline unsigned int GetOffset(int i) const { return m_Offset[i]; }

private:
	std::vector<VertexLayoutElement> m_Elements;
	std::vector<unsigned int> m_Offset;
	unsigned int m_Stride;
};