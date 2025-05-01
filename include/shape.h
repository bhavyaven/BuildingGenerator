#pragma once
#include <glm/glm.hpp>
#include <string>

struct Shape {
	glm::vec3 pos;
	glm::vec3 size;
	glm::vec3 color;
	std::string type;

	Shape() : pos(0.0f), size(1.0f), color(1.0f), type("") {}
};