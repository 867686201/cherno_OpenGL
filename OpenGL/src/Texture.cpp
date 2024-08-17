#include "Texture.h"
#include "stb_image/stb_image.h"
#include "GLError.h"

Texture::Texture(const std::string& filePath)					
	: m_RendererID(0), m_TexBuffer(nullptr), m_FilePath(filePath),
	m_Width(0), m_Height(0), m_Channels(0), m_Slot(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_TexBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_Channels, 4);

	CreateTexture();

	if (m_TexBuffer) {
		stbi_image_free(m_TexBuffer);		// 释放 stbi_load
	}
}

Texture::Texture(unsigned char* buffer, int width, int height)			
	: m_RendererID(0), m_TexBuffer(buffer), m_FilePath(),
	m_Width(width), m_Height(height), m_Channels(4), m_Slot(0)
{
	CreateTexture();
}

void Texture::CreateTexture()
{
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//                                mipmap_level					宽度(必须为0)
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TexBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::UpdateTexture(unsigned char* buffer)
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	// 更新已经存在的纹理对象的一个子区域（或全部）。它不重新分配内存，而是使用已有的内存块并更新纹理的部分内容
	//                             mipmap_level  xy 偏移  xy 范围
	GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot)
{
	m_Slot = slot;
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

