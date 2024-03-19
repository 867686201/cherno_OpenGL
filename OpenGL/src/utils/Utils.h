#pragma once

#include <glm/glm.hpp>
#include <functional>

namespace utils{
	glm::mat4 lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

	extern std::function<void(const glm::mat4&)> printMat4;
	extern std::function<void(const glm::mat4&, const glm::mat4&)> printEqual;

}