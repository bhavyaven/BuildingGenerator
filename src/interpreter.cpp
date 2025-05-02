#include "interpreter.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

static std::string trim(const std::string& line) {
    const std::string ws = " \t\r\n";
    auto first = line.find_first_not_of(ws);
    if (first == std::string::npos) return "";
    auto last = line.find_last_not_of(ws);
    return line.substr(first, last - first + 1);
}

void Interpreter::loadGrammar(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open grammar file: " << filename << std::endl;
        return;
    }

    std::string line;
    char currentSymbol = ' ';
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (line.find("param") == 0) {
            size_t eq = line.find('=');
            std::string key = trim(line.substr(6, eq - 6));
            float val = std::stof(trim(line.substr(eq + 1)));
            params[key] = val;
        }
        else if (line.find("shape") == 0) {
            currentSymbol = trim(line.substr(6))[0];
        }
        else {
            GrammarRule rule;
            if (line.find("split") == 0) {
                size_t open = line.find('(');
                size_t close = line.find(')');
                rule.op = "split";
                rule.axis = line.substr(open + 1, close - open - 1);

                size_t brace = line.find('{');
                size_t end = line.find('}');
                std::string contents = line.substr(brace + 1, end - brace - 1);
                std::istringstream ss(contents);
                std::string tok;
                while (std::getline(ss, tok, '|')) {
                    size_t colon = tok.find(':');
                    float weight = std::stof(trim(tok.substr(0, colon)));
                    char sym = trim(tok.substr(colon + 1))[0];
                    rule.entries.push_back({ weight, sym });
                }
            }
            else if (line.find("scale") == 0) {
                size_t open = line.find('(');
                size_t close = line.find(')');
                rule.op = "scale";
                rule.axis = line.substr(open + 1, close - open - 1);
                rule.scaleFactor = std::stof(trim(line.substr(close + 1)));
            }

            rules[currentSymbol] = rule;
        }
    }
}

void Interpreter::expand(const Shape& shape, std::vector<Shape>& out) {
    auto it = rules.find(shape.symbol);
    if (it == rules.end()) {
        out.push_back(shape);  // Terminal shape
        return;
    }

    const GrammarRule& rule = it->second;

    if (rule.op == "split") {
        float total = 0.0f;
        for (const RuleEntry& entry : rule.entries) total += entry.weight;

        float offset = 0.0f;
        for (const RuleEntry& entry : rule.entries) {
            float ratio = entry.weight / total;
            Shape child = shape;
            child.symbol = entry.symbol;
            child.scale = shape.scale;

            if (rule.axis == "z") {
                child.scale.z = shape.scale.z * ratio;
                child.position += shape.zAxis * (offset + child.scale.z / 2.0f);
                offset += child.scale.z;
            }
            else if (rule.axis == "x") {
                child.scale.x = shape.scale.x * ratio;
                child.position += shape.xAxis * (offset + child.scale.x / 2.0f);
                offset += child.scale.x;
            }

            out.push_back(child);
        }

    }
    else if (rule.op == "scale") {
        Shape child = shape;
        if (rule.axis == "y") {
            child.scale.y *= rule.scaleFactor;
        }
        child.symbol = 'T';  // Terminal
        out.push_back(child);
    }
}