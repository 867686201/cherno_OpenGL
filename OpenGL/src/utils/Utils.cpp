#include "Utils.h"
#include "Transform.h"

#include <iostream>
#include <iomanip> // 添加这个头文件以使用 std::fixed 和 std::setprecision
#include <glm/gtc/epsilon.hpp> // 包含epsilonEqual函数


namespace utils
{
	glm::mat4 lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
	{
		glm::vec3 look = center - eye;	// -z 方向
		glm::vec3 new_z = -glm::normalize(look); // new_z
        glm::vec3 new_x = glm::normalize(glm::cross(up, new_z));
		glm::vec3 new_y = glm::normalize(glm::cross(new_z, new_x));
		glm::mat4 rotate(1);
        rotate[0] = glm::vec4(new_x, 0.0f);
        rotate[1] = glm::vec4(new_y, 0.0f);
        rotate[2] = glm::vec4(new_z, 0.0f);
        rotate = glm::transpose(rotate);
        glm::mat4 translate = translation(-eye.x, -eye.y, -eye.z);
		return rotate * translate;
	}


    std::function<void(const glm::mat4&)> printMat4 = [](const glm::mat4& mat) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                // 限制小数点后最多两位
                std::cout << std::fixed << std::setprecision(2) << std::setw(7) << mat[j][i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        };

    std::function<void(const glm::mat4&, const glm::mat4&)> printEqual = [](const glm::mat4& mat1, const glm::mat4& mat2) {
        float epsilon = 0.0001f;
        bool equal = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (!glm::epsilonEqual(mat1[i][j], mat2[i][j], epsilon)) {
                    equal = false;
                    std::cout << equal << std::endl;
                    return;
                }
            }
        }
        equal = true;
        std::cout << equal << std::endl;
        return;
        };

}