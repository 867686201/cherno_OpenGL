#pragma once
#include "tgaimage.h"
#include <functional>
#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//定义宽度高度
const int width = 800;
const int height = 800;

// 定义颜色
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor black = TGAColor(0, 0, 0, 255);


bool is_inner(const std::vector<glm::vec3>& v, const glm::vec3& p);
glm::vec3 barycentric(const std::vector<glm::vec3>& v, const glm::vec3& p);

glm::vec3 barycentric_by_dot(const std::vector<glm::vec3>& vert, const glm::vec3& p);

void triangle(const std::vector<glm::vec3>& v, const std::vector<glm::vec3>& world_coords, std::vector<double>& zbuffer, TGAImage& image, glm::vec3 color);