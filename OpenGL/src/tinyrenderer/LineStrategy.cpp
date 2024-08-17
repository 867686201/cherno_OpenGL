#include "LineStrategy.h"

void BresenhamStrategy::drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) const
{
    bool steep = false;
    if (std::abs(x1 - x0) < std::abs(y1 - y0))     // k > 1, 需要按 y 轴循环
    {
        std::swap(x0, y0);                         // 如果需要按 y 轴循环, 交换后, 这里的 x 代表的就是 y
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror = 2 * std::abs(dy);
    int error = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        if (steep)
            image.set(y, x, color);
        else
            image.set(x, y, color);
        error += derror;
        if (error > dx)
        {
            y += y1 > y0 ? 1 : -1;
            error -= 2 * dx;
        }
    }
}

void DDAStrategy::drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) const
{
    float x, y;
    float dx = x1 - x0;
    float dy = y1 - y0;
    float n = std::max(std::abs(dx), std::abs(dy));
    dx = dx / n;  // 一个为 1，一个为小数
    dy = dy / n;

    x = x0;
    y = y0;
    for (int i = 0; i <= n; i++)
    {
        //image.set(std::round(x), std::round(y), color);
        image.set(static_cast<int>(x + 0.5), static_cast<int>(y + 0.5), color);
        x += dx;
        y += dy;
    }
}

void LinearInterpolationStrategy::drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) const
{
    bool steep = false;
    if (std::abs(x1 - x0) < std::abs(y1 - y0))     // k > 1, 需要按 y 轴循环
    {
        std::swap(x0, y0);                         // 如果需要按 y 轴循环, 交换后, 这里的 x 代表的就是 y
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    for (int x = x0; x <= x1; x++)
    {
        float t = float(x - x0) / (x1 - x0);
        int y = y0 + (y1 - y0) * t;
        if (steep)
            image.set(y, x, color);
        else
            image.set(x, y, color);

    }
}
