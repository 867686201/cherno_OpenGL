#include "Texture.h"
#include "ThirdParty/stb_image/stb_image.h"
#include "GLError.h"

Texture::Texture(const std::string& filePath)
	: m_RendererID(0), m_TexBuffer(nullptr), m_FilePath(filePath),
	m_Width(0), m_Height(0), m_Channels(0), m_Slot(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_TexBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_Channels, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TexBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_TexBuffer) {
		stbi_image_free(m_TexBuffer);
	}
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
