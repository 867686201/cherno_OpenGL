#pragma once

#include "tgaimage.h"
#include "model.h"
#include "Util.h"
#include "LineDrawer.h"

#include <vector>
#include <memory>
#include <string>


class TinyRenderer {
public:
    TinyRenderer(); // 构造函数
    TinyRenderer(int width, int height); // 构造函数

    //void initialize(int width, int height); // 初始化渲染器

    void setModel(const std::string& filePath); // 设置模型

    void render(); // 渲染场景
    void renderLine(LineAlgorithm algo = LineAlgorithm::Bresenham);

    void addLine(const glm::vec2& v0, const glm::vec2& v1, TGAColor color) { m_Lines.emplace_back(v0, v1, color); };

    void clearFrameBuffer(glm::vec4 color); // 清空帧缓冲区

    void writeFile(std::string path = "out.tga");

    void setViewPos(const glm::vec3& pos) { m_viewPos = pos; }; // 设置光源
    void addLight(const glm::vec4& light) { m_lights.push_back(light); }; // 设置光源
    unsigned char* getFrameBufferData() const { return m_frameBuffer.get()->buffer(); }; // 获取帧缓冲区数据指针
    inline int getWidth() const { return m_width; }; // 获取渲染目标的宽度
    inline int getHeight() const { return m_height; };; // 获取渲染目标的高度

private:
    glm::vec3 worldToScreen(const glm::vec3& vert) const 
    { return glm::vec3((vert.x + 1.0f) * m_width / 2.0 + 0.5, (vert.y + 1.0f) * m_height / 2.0 + 0.5, vert.z); }; // 将世界坐标转换为屏幕坐标
    glm::vec3 calculateLighting(const glm::vec3& point, const glm::vec3& normal) const; // 计算光照

    int m_width, m_height;
    std::shared_ptr<Model> m_model;
    glm::vec3 m_viewPos;
    std::vector<glm::vec4> m_lights;
    std::vector<Line> m_Lines;
    std::unique_ptr<TGAImage> m_frameBuffer;
    std::vector<double> m_zbuffer;
};

