#include "LineStrategyFactory.h"




LineStrategy* StrategyFactory::NewStrategy(LineAlgorithm algorithm) const {
    switch (algorithm) {
    case LineAlgorithm::Bresenham:
        return new BresenhamStrategy();
    case LineAlgorithm::DDA:
        return new DDAStrategy();
    case LineAlgorithm::LinearInterpolation:
        return new LinearInterpolationStrategy();
    default:
        return nullptr;
    }
}
