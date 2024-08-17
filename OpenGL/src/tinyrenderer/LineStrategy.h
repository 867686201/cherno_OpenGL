#pragma once
#include "tgaimage.h"

// 抽象策略接口
class LineStrategy {
public:
    virtual void drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) const = 0;   // 纯虚函数
    virtual ~LineStrategy() = default;
};

class BresenhamStrategy : public LineStrategy {
public:
    void drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) const override;
};

class DDAStrategy : public LineStrategy {
public:
    void drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) const override;
};

class LinearInterpolationStrategy : public LineStrategy {
public:
    void drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) const override;
};
