#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
using namespace std;

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

glm::mat4 rotate(Dimension dim, Axis axis, float angle)
{
    glm::mat4 rotate_matrix(1);
    angle = glm::radians(angle); //角度值转弧度制
    if (dim == Dimension::dim_2D)
    {
        rotate_matrix[0] = glm::vec4(glm::cos(angle), glm::sin(angle), 0, 0);
        rotate_matrix[1] = glm::vec4(-glm::sin(angle), glm::cos(angle), 0, 0);
        rotate_matrix[2] = glm::vec4(0, 0, 0, 0);
    }

    if (dim == Dimension::dim_3D)
    {
        if (axis == Axis::m_x_axis)
        {
            rotate_matrix[0] = glm::vec4(1, 0, 0, 0);
            rotate_matrix[1] = glm::vec4(0, glm::cos(angle), glm::sin(angle), 0);
            rotate_matrix[2] = glm::vec4(0, -glm::sin(angle), glm::cos(angle), 0);
        }
        else if (axis == Axis::m_y_axis)
        {
            rotate_matrix[0] = glm::vec4(glm::cos(angle), 0, -glm::sin(angle), 0);
            rotate_matrix[1] = glm::vec4(0, 1, 0, 0);
            rotate_matrix[2] = glm::vec4(glm::sin(angle), 0, glm::cos(angle), 0);
        }
        else if (axis == Axis::m_z_axis)
        {
            rotate_matrix[0] = glm::vec4(glm::cos(angle), glm::sin(angle), 0, 0);
            rotate_matrix[1] = glm::vec4(-glm::sin(angle), glm::cos(angle), 0, 0);
            rotate_matrix[2] = glm::vec4(0, 0, 1, 0);
        }
    }

    return rotate_matrix;
}

glm::mat4 scale(float x, float y, float z)
{
    glm::mat4 scale_matrix = 
    {
        x,    0.0f, 0.0f, 0.0f,
        0.0f, y,    0.0f, 0.0f,
        0.0f, 0.0f, z   , 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return scale_matrix;
}

glm::mat4 translation(float x, float y, float z)
{
    glm::mat4 translation_matrix{ glm::mat4(1) };
    translation_matrix[3] = { x, y, z, 1 };             // 列主序, 直接通过 [3] 写入最后一列
    return translation_matrix;
}

// 二维绕任意点旋转
glm::mat4 arbitrary_2D_rotation(float angle, float x, float y)
{
    glm::mat4 arbitrary_2D_rotation_matrix =
        translation(x, y, 0) * rotate(Dimension::dim_2D, Axis::origin, angle) * translation(-x, -y, 0);
    return arbitrary_2D_rotation_matrix;
}

void similarity_matrix_generate(glm::mat4& P, glm::mat4& P_T, float x, float y, float z)
{
    glm::vec3 rotate_axis{ x, y, z };
    glm::vec3 new_z = glm::normalize(rotate_axis);
    glm::vec3 new_y = glm::normalize(glm::cross(new_z, glm::vec3(0.0f, 0.0f, 1.0f)));   // normalize( new_z cross  )
    glm::vec3 new_x = glm::normalize(glm::cross(new_y, new_z)) ;

    P[0] = glm::vec4(new_x, 0.0f);  // 旧坐标到新坐标的基向量作为列向量
    P[1] = glm::vec4(new_y, 0.0f);
    P[2] = glm::vec4(new_z, 0.0f);

    P_T = glm::transpose(P); 
}

// 三维绕任意轴旋转 任意轴过原点
glm::mat4 arbitrary_3D_rotation(float angle, float x, float y, float z)
{
    glm::mat4 P_T(1), P(1);
    similarity_matrix_generate(P, P_T, x, y, z);
    glm::mat4 arbitrary_3D_rotation_matrix = P * rotate(Dimension::dim_3D, Axis::m_z_axis, angle) * P_T;

    return arbitrary_3D_rotation_matrix;
}

// 三维绕任意轴缩放 任意轴过原点
glm::mat4 arbitrary_3D_scale(float scale_x, float scale_y, float scale_z, float x, float y, float z)
{
    glm::mat4 P_T(1), P(1);
    similarity_matrix_generate(P, P_T, x, y, z);
    glm::mat4 arbitrary_3D_scale_matrix = P * scale(scale_x, scale_y, scale_z) * P_T;

    return arbitrary_3D_scale_matrix;
}

// rodrigues' rotation formula
glm::mat4 rodrigues_rotate(float angle, float x, float y, float z)
{
    float length = std::sqrt(x * x + y * y + z * z);
    if (length != 0)
    {
        x /= length;
        y /= length;
        z /= length;
    }
    angle = glm::radians(angle); //角度值转弧度制
    glm::mat3 rodrigues_rotate_matrix_0{
        glm::cos(angle), 0, 0,
        0, glm::cos(angle), 0,
        0, 0, glm::cos(angle)
    };
    glm::mat3 rodrigues_rotate_matrix_1 = glm::outerProduct(glm::vec3(x, y, z), glm::vec3(x, y, z)) * (1 - glm::cos(angle));
    glm::mat3 rodrigues_rotate_matrix_2{
        0, -z, y,
        z, 0, -x,
        -y, x, 0
    };
    rodrigues_rotate_matrix_2 *= glm::sin(angle);
    rodrigues_rotate_matrix_0 = rodrigues_rotate_matrix_0 + rodrigues_rotate_matrix_1 + rodrigues_rotate_matrix_2;

    glm::mat4 rodrigues_rotate_matrix(1.0f); // 创建一个4x4矩阵并初始化为单位矩阵

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            rodrigues_rotate_matrix[i][j] = rodrigues_rotate_matrix_0[i][j];
        }
    }
    return rodrigues_rotate_matrix;
}


