#pragma once

#include "shape.hpp"
#include <functional>
#include <vector>

struct Rule {
    float probability;
    std::function<void(Shape&, std::vector<Shape>&)> func;

    Rule(float prob, std::function<void(Shape&, std::vector<Shape>&)> f)
        : probability(prob), func(f) {
    }
};