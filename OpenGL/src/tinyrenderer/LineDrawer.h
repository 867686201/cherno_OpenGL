#pragma once
#include "LineStrategy.h"
#include "LineStrategyFactory.h"
#include "glm/glm.hpp"
#include <vector>

struct Line
{
    int x0;
    int y0;
    int x1;
    int y1;
    TGAColor color;

    Line(int _x0, int _y0, int _x1, int _y1, TGAColor _color)
        : x0(_x0), y0(_y0), x1(_x1), y1(_y1), color(_color)
    {}
    Line(glm::vec2 v0, glm::vec2 v1, TGAColor _color)
        : x0(static_cast<int>(v0.x)), y0(static_cast<int>(v0.y)),
        x1(static_cast<int>(v1.x)), y1(static_cast<int>(v1.y)), color(_color)
    {}
};

// LineDrawer ¿‡
class LineDrawer {
private:
    LineStrategy* strategy;

public:
    LineDrawer(StrategyFactory* strategyFactory, LineAlgorithm algorithm);
    ~LineDrawer();

    void draw(const std::vector<Line>& lines, TGAImage& image);
};