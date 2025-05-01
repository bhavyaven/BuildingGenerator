#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "shape.h"
#include "rule.h"

class Interpreter {
public:
	void loadGrammar(const std::string& fileName);
	void executeRule(const std::string& ruleName, Shape shape = Shape());
	void initOpenGL();
	void run();

private:
	std::unordered_map<std::string, Rule> rules;
	std::unordered_map<std::string, std::string> params;
	std::unordered_map<std::string, std::string> constants;
	std::vector<Shape> shapes;

	unsigned int shader;
	unsigned int VAO, VBO;

	glm::mat4 view;
	glm::mat4 projection;

	void setupCube();
	void loadShaders();
	void drawShape(const Shape& shape);
};