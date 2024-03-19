#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

enum class Dimension
{
    dim_2D,
    dim_3D
};

enum class Axis
{
    origin,     // 2D 
    m_x_axis,
    m_y_axis,
    m_z_axis
};

glm::mat4 rotate(Dimension dim, Axis axis, float angle);

glm::mat4 scale(float x, float y, float z);

glm::mat4 translation(float x, float y, float z);

// 二维绕任意点旋转
glm::mat4 arbitrary_2D_rotation(float angle, float x, float y);

void similarity_matrix_generate(glm::mat4& P, glm::mat4& P_T, float x, float y, float z);

// 三维绕任意轴旋转 任意轴过原点
glm::mat4 arbitrary_3D_rotation(float angle, float x, float y, float z);

// 三维绕任意轴缩放 任意轴过原点
glm::mat4 arbitrary_3D_scale(float scale_x, float scale_y, float scale_z, float x, float y, float z);

// rodrigues' rotation formula
glm::mat4 rodrigues_rotate(float angle, float x, float y, float z);
