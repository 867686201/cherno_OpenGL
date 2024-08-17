#include "LineDrawer.h"

LineDrawer::LineDrawer(StrategyFactory* strategyFactory, LineAlgorithm algorithm) {
    this->strategy = strategyFactory->NewStrategy(algorithm);
}

LineDrawer::~LineDrawer() {
    delete this->strategy;
}

void LineDrawer::draw(const std::vector<Line>& lines, TGAImage& image) {
    if (strategy) {
        for (const auto& line : lines) {
            auto [x0, y0, x1, y1, color] = line;
            strategy->drawLine(x0, y0, x1, y1, image, color);
        }
    }
}