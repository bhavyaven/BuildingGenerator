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

bool isTerminal(const std::string& sym) {
    return sym.size() > 0 && sym[0] == 'T';
}

void Interpreter::loadGrammar(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open grammar file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string currentSymbol;
    while (std::getline(file, line)) {
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
            currentSymbol = trim(line.substr(6));
            rules[currentSymbol] = {};
        }
        else if (line.find("scale") == 0 || line.find("translate") == 0 || line.find("add_door") == 0 || line.find("add_chimney") == 0 || line.find("split") == 0) {
            GrammarRule rule;

            if (line.find("scale") == 0) {
                size_t open = line.find('(');
                size_t close = line.find(')');
                rule.op = "scale";
                rule.axis = line.substr(open + 1, close - open - 1);
                rule.scaleFactor = std::stof(trim(line.substr(close + 1)));
            }
            else if (line.find("translate") == 0) {
                size_t open = line.find('(');
                size_t close = line.find(')');
                rule.op = "translate";
                rule.axis = line.substr(open + 1, close - open - 1);
                rule.scaleFactor = std::stof(trim(line.substr(close + 1)));
            }
            else if (line.find("add_door") == 0) {
                rule.op = "add_door";
            }
            else if (line.find("add_chimney") == 0) {
                rule.op = "add_chimney";
                size_t open = line.find('(');
                size_t close = line.find(')');
                rule.axis = trim(line.substr(open + 1, close - open - 1));
            }
            else if (line.find("split") == 0) {
                rule.op = "split";
                size_t open = line.find('(');
                size_t close = line.find(')');
                rule.axis = line.substr(open + 1, close - open - 1);

                size_t brace = line.find('{');
                size_t end = line.find('}');
                std::string contents = line.substr(brace + 1, end - brace - 1);
                std::istringstream ss(contents);
                std::string tok;
                while (std::getline(ss, tok, '|')) {
                    size_t colon = tok.find(':');
                    float weight = std::stof(trim(tok.substr(0, colon)));
                    std::string sym = trim(tok.substr(colon + 1));
                    rule.entries.push_back({ weight, sym[0] });
                }
            }
            rules[currentSymbol].push_back(rule);
        }
    }
}

void Interpreter::expand(const Shape& shape, std::vector<Shape>& out) {
    std::string sym(1, shape.symbol);

    if (isTerminal(sym)) {
        out.push_back(shape);
        return;
    }

    auto it = rules.find(sym);
    if (it == rules.end()) {
        out.push_back(shape);
        return;
    }

    Shape working = shape;
    for (const GrammarRule& rule : it->second) {
        if (rule.op == "scale") {
            if (rule.axis == "x") working.scale.x *= rule.scaleFactor;
            if (rule.axis == "y") {
                float originalYScale = shape.scale.y;
                float newYScale = originalYScale * rule.scaleFactor;
                float deltaY = (newYScale - originalYScale) / 2.0f;
                working.position += glm::vec3(0, deltaY, 0);
                working.scale.y = newYScale;
            }
            if (rule.axis == "z") {
                working.scale.z *= rule.scaleFactor;
            }
            if (rule.axis == "uniform") working.scale *= rule.scaleFactor;
        }
        else if (rule.op == "translate") {
            if (rule.axis == "x") working.position += working.xAxis * rule.scaleFactor;
            else if (rule.axis == "y") working.position += glm::vec3(0, 1, 0) * rule.scaleFactor;
            else if (rule.axis == "z") working.position += working.zAxis * rule.scaleFactor;
        }
        else if (rule.op == "add_door") {
            working.hasDoor = true;
        }
        else if (rule.op == "add_chimney") {
            working.hasChim = true;
            working.chimSide = rule.axis.c_str();
        } 
        
    }

    for (const GrammarRule& rule : it->second) {
        if (rule.op == "split") {
            float totalWeight = 0.0f;
            for (const auto& entry : rule.entries) totalWeight += entry.weight;

            float startOffset = -0.5f;
            float offset = startOffset;

            for (const auto& entry : rule.entries) {
                float ratio = entry.weight / totalWeight;
                Shape child = working;
                child.symbol = entry.symbol;

                if (rule.axis == "x") {
                    child.scale.x = shape.scale.x * ratio;
                    float shift = shape.scale.x * (offset + ratio / 2.0f);
                    child.position += shape.xAxis * shift;
                }
                else if (rule.axis == "z") {
                    child.scale.z = shape.scale.z * ratio;
                    float shift = shape.scale.z * (offset + ratio / 2.0f);
                    child.position += shape.zAxis * shift;
                }
                offset += ratio;
                out.push_back(child);
            }
            return;
        }
    }
    out.push_back(working);
}