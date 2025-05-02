#include "mesh.hpp"

void Mesh::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

//Mesh createCubeMesh() {
//    float vertices[] = {
//    -0.5f, -0.5f, -0.5f,
//     0.5f, -0.5f, -0.5f,
//     0.5f,  0.5f, -0.5f,
//    -0.5f,  0.5f, -0.5f,
//    -0.5f, -0.5f,  0.5f,
//     0.5f, -0.5f,  0.5f,
//     0.5f,  0.5f,  0.5f,
//    -0.5f,  0.5f,  0.5f
//    };
//
//    unsigned int indices[] = {
//    0, 1, 2, 2, 3, 0, // Front face
//    1, 5, 6, 6, 2, 1, // Right face
//    3, 2, 6, 6, 7, 3, // Top face
//    4, 0, 3, 3, 7, 4, // Left face
//    4, 5, 1, 1, 0, 4, // Bottom face
//    5, 4, 7, 7, 6, 5  // Back face
//    };
//
//    Mesh mesh;
//    mesh.indexCount = sizeof(indices) / sizeof(unsigned int);
//
//    glGenVertexArrays(1, &mesh.VAO);
//    glGenBuffers(1, &mesh.VBO);
//    glGenBuffers(1, &mesh.EBO);
//
//    glBindVertexArray(mesh.VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
//    glEnableVertexAttribArray(1);
//
//    return mesh;
//}

Mesh createCubeMesh() {
    std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

        // Back face
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},

        // Left face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

        // Right face
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},

        // Top face
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},

        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    };

    std::vector<GLuint> indices;
    for (GLuint i = 0; i < 24; i += 4) {
        indices.insert(indices.end(), { i, i + 1, i + 2, i, i + 2, i + 3 });
    }

    Mesh mesh;
    mesh.indexCount = static_cast<GLuint>(indices.size());

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return mesh;
}

Mesh createRoofMesh() {
    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},  // front triangle
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.0f,  0.5f,  0.5f}, {0.5f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},  // back triangle
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.0f,  0.5f, -0.5f}, {0.5f, 1.0f}},
    };

    std::vector<GLuint> indices = {
        0, 1, 2,
        3, 4, 5,
        0, 2, 3, 2, 3, 5,
        1, 4, 2, 2, 4, 5,
        0, 1, 3, 1, 3, 4
    };

    Mesh mesh;
    mesh.indexCount = static_cast<GLuint>(indices.size());

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return mesh;
}



//Mesh createRoofMesh() {
//    std::vector<Vertex> vertices = {
//        {{-0.5f, -0.5f,  0.5f}}, // 0 Front left
//        {{ 0.5f, -0.5f,  0.5f}}, // 1 Front right
//        {{ 0.0f,  0.5f,  0.5f}}, // 2 Front top
//        {{-0.5f, -0.5f, -0.5f}}, // 3 Back left
//        {{ 0.5f, -0.5f, -0.5f}}, // 4 Back right
//        {{ 0.0f,  0.5f, -0.5f}}  // 5 Back top
//    };
//
//    std::vector<GLuint> indices = {
//        0, 1, 2, // front triangle
//        3, 4, 5, // back triangle
//        0, 2, 3, 2, 3, 5, // left side
//        1, 4, 2, 2, 4, 5, // right side
//        0, 1, 3, 1, 3, 4  // bottom
//    };
//
//    Mesh mesh;
//    mesh.indexCount = static_cast<unsigned int>(indices.size());
//
//    glGenVertexArrays(1, &mesh.VAO);
//    glGenBuffers(1, &mesh.VBO);
//    glGenBuffers(1, &mesh.EBO);
//
//    glBindVertexArray(mesh.VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
//    glEnableVertexAttribArray(1);
//    
//    return mesh;
//}
