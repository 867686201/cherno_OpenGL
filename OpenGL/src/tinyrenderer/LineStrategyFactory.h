#pragma once
#include "LineStrategy.h"

enum class LineAlgorithm {
    Bresenham,
    DDA,
    LinearInterpolation
};

// 策略工厂类
class StrategyFactory {
public:
    LineStrategy* NewStrategy(LineAlgorithm algorithm) const;

};
