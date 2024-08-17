#pragma once

#include <string>

class Texture
{
public:
	Texture(const std::string& filePath);
	Texture(unsigned char* buffer, int width, int height);
	~Texture();

	void Bind(unsigned int slot = 0);  
	void Unbind() const;

	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
	inline int GetSlot() { return m_Slot; }

	void UpdateTexture(unsigned char* buffer);
private:
	void CreateTexture();

	unsigned int m_RendererID;
	unsigned char* m_TexBuffer;
	std::string m_FilePath;
	int m_Width, m_Height;
	int m_Channels; // bits per pixel
	unsigned int m_Slot;
};