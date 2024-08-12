#include "Util.h"


bool is_inner(const std::vector<glm::vec3>& v, const glm::vec3& p)
{
    glm::vec3 AB = v[1] - v[0];
    glm::vec3 BC = v[2] - v[1];
    glm::vec3 CA = v[0] - v[2];
    glm::vec3 AP = p - v[0];
    glm::vec3 BP = p - v[1];
    glm::vec3 CP = p - v[2];

    glm::vec3 normal = glm::cross(AB, -CA);

    double signA = glm::dot(glm::cross(AB, AP), normal);
    double signB = glm::dot(glm::cross(BC, BP), normal);
    double signC = glm::dot(glm::cross(CA, CP), normal);

    return (signA * signB >= 0) && (signA * signC >= 0);
}


glm::vec3 barycentric(const std::vector<glm::vec3>& v, const glm::vec3& p)
{
    double areaA;
    double areaB;
    double areaC;
    glm::vec3 AP = p - v[0];
    glm::vec3 BP = p - v[1];
    glm::vec3 CP = p - v[2];

    glm::vec3 AB = v[1] - v[0];
    glm::vec3 AC = v[2] - v[0];

    glm::vec3 normal = glm::cross(AB, AC);

    areaA = glm::length(glm::cross(BP, CP)) * (glm::dot(glm::cross(BP, CP), normal) >= 0 ? 1 : -1);
    areaB = glm::length(glm::cross(CP, AP)) * (glm::dot(glm::cross(CP, AP), normal) >= 0 ? 1 : -1);
    areaC = glm::length(glm::cross(AP, BP)) * (glm::dot(glm::cross(AP, BP), normal) >= 0 ? 1 : -1);
    double area = areaA + areaB + areaC;
    return glm::vec3(areaA / area, areaB / area, areaC / area);

}

glm::vec3 barycentric_by_dot(const std::vector<glm::vec3>& vert, const glm::vec3& p)
{
    glm::vec3 v0 = vert[1] - vert[0];
    glm::vec3 v1 = vert[2] - vert[0];
    glm::vec3 v2 = p - vert[0];

    double d00 = glm::dot(v0, v0);
    double d01 = glm::dot(v0, v1);
    double d11 = glm::dot(v1, v1);
    double d20 = glm::dot(v2, v0);
    double d21 = glm::dot(v2, v1);
    double denom = d00 * d11 - d01 * d01;
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w;
    return glm::vec3(u, v, w);
}

void triangle(const std::vector<glm::vec3>& v, const std::vector<glm::vec3>& world_coords, std::vector<double>& zbuffer, TGAImage& image, glm::vec3 color)
{
    glm::vec2 bboxmax(0, 0);
    glm::vec2 bboxmin(width - 1, height - 1);
    glm::vec2 clamp(width - 1, height - 1);
    for (int i = 0; i < 3; i++)
    {
        // 通过 (0, 0) 和 clamp 限制在窗口范围内
        bboxmin.x = std::max(0.f, std::min(v[i].x, bboxmin.x));
        bboxmin.y = std::max(0.f, std::min(v[i].y, bboxmin.y));

        bboxmax.x = std::min(clamp.x, std::max(v[i].x, bboxmax.x));
        bboxmax.y = std::min(clamp.y, std::max(v[i].y, bboxmax.y));
    }
    for (int i = bboxmin.x; i < bboxmax.x; i++)
    {
        for (int j = bboxmin.y; j < bboxmax.y; j++)
        {
            glm::vec3 p{ i, j, 0.0f };
            glm::vec3 bary_coord = barycentric(v, p);
            if (bary_coord.x >= 0 && bary_coord.y >= 0 && bary_coord.z >= 0) // 在三角形内部
            {
                glm::vec4 result_color(0.0f, 0.0f, 0.0f, 1.0f);
                // 环境光 ambient
                // La = ka * Ia


                // 漫反射 
                // Ls = ks(I/r^2) max(0, \vec n * \vec h)^p
                // h 为 Halfway Vector


                // 高光项
                // Ld = kd(I/r^2) max(0, \vec n * \vec l)

                p.z = 0;
                for (int k = 0; k < 3; k++)
                {
                    p.z += v[k][2] * bary_coord[k];     // 利用重心坐标插值出 z, p.z = A.z * alpha + B.z * beta + C.z * gamma
                }
                if (p.z > zbuffer[i + j * width])       // 如果新的一个像素更近, 则更新
                {
                    zbuffer[i + j * width] = p.z;
                    TGAColor setedColor(color.r, color.g, color.b, 255);
                    image.set(i, j, setedColor);
                }
            }
        }
    }
}