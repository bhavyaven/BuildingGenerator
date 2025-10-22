#pragma once
#include "shape.hpp"
#include <vector>
#include <string>
#include <unordered_map>

struct RuleEntry {
    float weight;
    char symbol;
};

struct GrammarRule {
    std::string op;
    std::string axis;
    std::vector<RuleEntry> entries;
    float scaleFactor = 1.0f;
};

class Interpreter {
public:
    void loadGrammar(const std::string& filename);
    void expand(const Shape& shape, std::vector<Shape>& out);
    std::unordered_map<std::string, float> params;

private:
    std::unordered_map<std::string, std::vector<GrammarRule>> rules;
};
