#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace std;

enum class Dimension
{
    dim_2D,
    dim_3D
};

enum class Axis
{
    origin, // 2D 
    m_x_axis,
    m_y_axis,
    m_z_axis
};

glm::mat4 rotate(Dimension dim, Axis axis, float angle)
{
    glm::mat4 rotate_matrix;
    angle = glm::radians(angle); //角度值转弧度制
    if (dim == Dimension::dim_2D)
    {
        rotate_matrix[0] = glm::vec4(glm::cos(angle), -glm::sin(angle), 0, 0);
        rotate_matrix[1] = glm::vec4(glm::sin(angle), glm::cos(angle), 0, 0);
        rotate_matrix[2] = glm::vec4(0, 0, 0, 0);
    }

    if (dim == Dimension::dim_3D)
    {
        if (axis == Axis::m_x_axis)
        {
            rotate_matrix[0] = glm::vec4(1, 0, 0, 0);
            rotate_matrix[1] = glm::vec4(0, glm::cos(angle), -glm::sin(angle), 0);
            rotate_matrix[2] = glm::vec4(0, glm::sin(angle), glm::cos(angle), 0);
        }
        else if (axis == Axis::m_y_axis)
        {
            rotate_matrix[0] = glm::vec4(glm::cos(angle), 0, -glm::sin(angle), 0);
            rotate_matrix[1] = glm::vec4(0, 1, 0, 0);
            rotate_matrix[2] = glm::vec4(glm::sin(angle), 0, glm::cos(angle), 0);
        }
        else if (axis == Axis::m_z_axis)
        {
            rotate_matrix[0] = glm::vec4(glm::cos(angle), -glm::sin(angle), 0, 0);
            rotate_matrix[1] = glm::vec4(glm::sin(angle), glm::cos(angle), 0, 0);
            rotate_matrix[2] = glm::vec4(0, 0, 1, 0);
        }
    }

    rotate_matrix[3] = glm::vec4(0, 0, 0, 1);
    return rotate_matrix;
}

glm::mat4 scale(float x, float y, float z)
{
    glm::mat4 scale_matrix = {
        x,    0.0f, 0.0f, 0.0f,
        0.0f, y,    0.0f, 0.0f,
        0.0f, 0.0f, z   , 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return scale_matrix;
}

glm::mat4 translation(float x, float y, float z)
{
    glm::mat4 translation_matrix = {
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return translation_matrix;
}

// 二维绕任意点旋转
glm::mat4 arbitrary_2D_rotation(float angle, float x, float y)
{
    glm::mat4 arbitrary_2D_rotation_matrix = glm::mat4(1);

    arbitrary_2D_rotation_matrix *= translation(-x, -y, 0);
    arbitrary_2D_rotation_matrix *= rotate(Dimension::dim_2D, Axis::origin, angle);
    arbitrary_2D_rotation_matrix *= translation(x, y, 0);

    return arbitrary_2D_rotation_matrix;
}

void similarity_matrix_generate(glm::mat4& P, glm::mat4& P_T, float x, float y, float z)
{
    glm::vec3 rotate_axis(x, y, z);
    glm::vec3 new_z = glm::normalize(rotate_axis);
    glm::vec3 new_y = glm::cross(new_z, glm::vec3(0.0f, 0.0f, 1.0f)) / glm::length(glm::cross(new_z, glm::vec3(0.0f, 0.0f, 1.0f)));
    glm::vec3 new_x = glm::cross(new_y, rotate_axis) / glm::length(glm::cross(new_y, rotate_axis));

    P_T = glm::mat4(     // 行向量为基向量
        glm::vec4(new_x, 0.0f),
        glm::vec4(new_y, 0.0f),
        glm::vec4(new_z, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
    P = glm::transpose(P_T); // 列向量为基向量
}

// 三维绕任意轴旋转 任意轴过原点
glm::mat4 arbitrary_3D_rotation(float angle, float x, float y, float z)
{
    glm::mat4 arbitrary_3D_rotation_matrix = glm::mat4(1);
    glm::mat4 P_T, P;
    similarity_matrix_generate(P, P_T, x, y, z);
    arbitrary_3D_rotation_matrix *= P_T;
    arbitrary_3D_rotation_matrix *= rotate(Dimension::dim_3D, Axis::m_z_axis, angle);
    arbitrary_3D_rotation_matrix *= P;

    return arbitrary_3D_rotation_matrix;
}

// 三维绕任意轴缩放 任意轴过原点
glm::mat4 arbitrary_3D_scale(float s_x, float s_y, float s_z, float x, float y, float z)
{
    glm::mat4 arbitrary_3D_scale_matrix = glm::mat4(1);
    glm::mat4 P_T, P;
    similarity_matrix_generate(P, P_T, x, y, z);
    arbitrary_3D_scale_matrix *= P_T;
    arbitrary_3D_scale_matrix *= scale(s_x, s_y, s_z);
    arbitrary_3D_scale_matrix *= P;

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

// 仿射变换
glm::mat4 affine_transform()
{
    glm::mat4 affine_transform_matrix = glm::mat4(1);

    affine_transform_matrix *= scale(0.8f, 0.5f, 0.5f);
    // affine_transform_matrix *= rotate(Dimension::dim_3D, Axis::m_y_axis, 30);
    // affine_transform_matrix *= translation(-0.5, -0.5, -0.5);

    return  affine_transform_matrix;
}


// 数学中矩阵相乘是右到左，而glm中是左到右
void transform(vector<glm::vec3>& vertices)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        // vertices[i] = glm::vec3(glm::vec4(vertices[i], 1.0f) * arbitrary_2D_rotation(30, 0.5, 0.5)); // 二维任意点旋转
        // vertices[i] = glm::vec3(glm::vec4(vertices[i], 1.0f) * affine_transform());  // 仿射变换
        vertices[i] = glm::vec3(glm::vec4(vertices[i], 1.0f) * arbitrary_3D_rotation(45, 1, 1, 5));  // 三维任意轴旋转
        // vertices[i] = glm::vec3(glm::vec4(vertices[i], 1.0f) * arbitrary_3D_scale(1, 0.5, 1, 1, 1, 0));  // 三维任意轴缩放
        // vertices[i] = glm::vec3(glm::vec4(vertices[i], 1.0f) * rodrigues_rotate(45, 1, 1, 5));  // 三维任意轴旋转 rodrigues_rotate
    }
}


// cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl << endl;