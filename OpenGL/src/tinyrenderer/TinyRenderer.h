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
    TinyRenderer(); // ���캯��
    TinyRenderer(int width, int height); // ���캯��

    //void initialize(int width, int height); // ��ʼ����Ⱦ��

    void setModel(const std::string& filePath); // ����ģ��

    void render(); // ��Ⱦ����
    void renderLine(LineAlgorithm algo = LineAlgorithm::Bresenham);

    void addLine(const glm::vec2& v0, const glm::vec2& v1, TGAColor color) { m_Lines.emplace_back(v0, v1, color); };

    void clearFrameBuffer(glm::vec4 color); // ���֡������

    void writeFile(std::string path = "out.tga");

    void setViewPos(const glm::vec3& pos) { m_viewPos = pos; }; // ���ù�Դ
    void addLight(const glm::vec4& light) { m_lights.push_back(light); }; // ���ù�Դ
    unsigned char* getFrameBufferData() const { return m_frameBuffer.get()->buffer(); }; // ��ȡ֡����������ָ��
    inline int getWidth() const { return m_width; }; // ��ȡ��ȾĿ��Ŀ��
    inline int getHeight() const { return m_height; };; // ��ȡ��ȾĿ��ĸ߶�

private:
    glm::vec3 worldToScreen(const glm::vec3& vert) const 
    { return glm::vec3((vert.x + 1.0f) * m_width / 2.0 + 0.5, (vert.y + 1.0f) * m_height / 2.0 + 0.5, vert.z); }; // ����������ת��Ϊ��Ļ����
    glm::vec3 calculateLighting(const glm::vec3& point, const glm::vec3& normal) const; // �������

    int m_width, m_height;
    std::shared_ptr<Model> m_model;
    glm::vec3 m_viewPos;
    std::vector<glm::vec4> m_lights;
    std::vector<Line> m_Lines;
    std::unique_ptr<TGAImage> m_frameBuffer;
    std::vector<double> m_zbuffer;
};

