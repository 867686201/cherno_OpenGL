#pragma once
#include "LineStrategy.h"

enum class LineAlgorithm {
    Bresenham,
    DDA,
    LinearInterpolation
};

// ���Թ�����
class StrategyFactory {
public:
    LineStrategy* NewStrategy(LineAlgorithm algorithm) const;

};
