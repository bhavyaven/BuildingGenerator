#pragma once

#include <glm/glm.hpp>

struct Material {
	glm::vec3 wallColor;
	glm::vec3 roofColor;
};

class Shape {
public:
    char symbol;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Material material;
    glm::vec3 xAxis, zAxis;
    bool hasDoor;
    bool hasChim;
    const char * chimSide;
    bool hasWind;
    const char* numWind;

    Shape(char sym, const glm::vec3& pos, const glm::vec3& rot,
        const glm::vec3& scl, const Material& mat,
        const glm::vec3& x, const glm::vec3& z, bool door,
        bool chim, const char * cSide);
};