#pragma once

#include <string>

#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderCode
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void SetUniform1i(const std::string& name, int value);
	void SetUniformMat4fv(const std::string& name, glm::mat4& mat);
private:
	ShaderCode ReadShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name) const;

	unsigned int m_RendererID;
	std::string m_Filepath;
	mutable std::unordered_map<std::string, int> m_UniformCache;
};