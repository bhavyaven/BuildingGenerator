#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 Position;
    glm::vec2 texCoord;
};

struct Mesh {
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
    void Draw();
};

Mesh createCubeMesh();
Mesh createRoofMesh();