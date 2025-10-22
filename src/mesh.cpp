#include "mesh.hpp"

void Mesh::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh createCubeMesh() {
    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},

        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},

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
        {{-0.55f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.55f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.0f,  0.2f,  0.5f}, {0.5f, 1.0f}},  
        
        {{-0.55f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.55f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.0f,  0.2f, -0.5f}, {0.5f, 1.0f}}, 

        {{-0.55f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.0f,  0.2f,  0.5f}, {1.0f, 0.0f}},
        {{-0.55f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.0f,  0.2f, -0.5f}, {1.0f, 1.0f}},

        {{ 0.55f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.0f,  0.2f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.55f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.0f,  0.2f, -0.5f}, {1.0f, 1.0f}},
    };

    std::vector<GLuint> indices = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        7, 8, 9,
        10,11,12,
        11,12,13 
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
