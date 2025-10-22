#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_set>

#include <../../external/stb/stb_image.h>
#include "shape.hpp"
#include "rule.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "interpreter.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 2.0f, 6.0f));
float deltaTime = 0.0f, lastFrame = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard('W', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard('S', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard('A', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard('D', deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void applyRule(const Rule& rule, Shape& parent, std::vector<Shape>& output) {
    std::vector<Shape> tempOutput;
    rule.func(parent, tempOutput);

    for (Shape& child : tempOutput) {
        child.hasDoor = parent.hasDoor;
        child.hasChim = parent.hasChim;
        child.chimSide = parent.chimSide;

        output.push_back(child);
    }
}

unsigned int loadTexture(const char* path, GLenum slot, GLenum format, GLenum pixelType) {
    unsigned int texID;
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(path, &widthImg, &heightImg, &numColCh, 0);
    if (!bytes) {
        std::cerr << "Failed to load texture at path: " << path << std::endl;
        return 0;
    }


    glGenTextures(1, &texID);
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texID;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Building Generator", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("../../shaders/v.glsl", "../../shaders/f.glsl");
    Interpreter interpreter;
    interpreter.loadGrammar("../../assets/config3-house.shp");

    Mesh cube = createCubeMesh();
    Mesh roof = createRoofMesh();

    Material mat = { glm::vec3(0.7f), glm::vec3(0.2f) };
    if (interpreter.params.count("width")) {
        float width = interpreter.params["width"];
    }
    else {
        std::cerr << "Missing width param in .shp file" << std::endl;
    }
    Shape base('S',
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(interpreter.params["width"], interpreter.params["height"], interpreter.params["depth"]),
        mat,
        glm::vec3(1, 0, 0),
        glm::vec3(0, 0, 1),
        false, false, "");

    std::unordered_set<char> expanded;
    std::vector<Shape> shapes = { base };
    for (int i = 0; i < 3; ++i) {
        std::vector<Shape> next;
        for (auto& s : shapes) {
            if (expanded.count(s.symbol) == 0) {
                interpreter.expand(s, next);
                expanded.insert(s.symbol);
            }
            else {
                next.push_back(s);
            }
        }
        shapes = next;
    }

    std::vector<Rule> rules;
    rules.emplace_back(1.0f, [](Shape& parent, std::vector<Shape>& output) {
        Shape front = parent;
        front.scale.z *= 0.5f;
        front.position += parent.zAxis * (parent.scale.z * 0.25f);
        output.push_back(parent);
        output.push_back(front);
        });

    std::vector<Shape> newShapes;
    for (Shape& s : shapes) {
        for (Rule& r : rules) applyRule(r, s, newShapes);
    }
    shapes = newShapes;

    shader.use();
    unsigned int wallTex = loadTexture("../../assets/siding-3.jpg", GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    unsigned int roofTex = loadTexture("../../assets/shingles-2.jpg", GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    unsigned int doorTex = loadTexture("../../assets/door.jpg", GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    unsigned int chimTex = loadTexture("../../assets/brick-2.jpg", GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);


    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        for (auto& shape : shapes) {

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(glm::mat4(1.0f), shape.position);
            model = glm::rotate(model, shape.rotation.y, glm::vec3(0, 1, 0));
            model = glm::scale(model, shape.scale);
            shader.setMat4("model", model);
            shader.setInt("texture1", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, wallTex);
            cube.Draw();

            glm::mat4 roofModel = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
            shader.setMat4("model", roofModel);
            glBindTexture(GL_TEXTURE_2D, roofTex);
            roof.Draw();
            
            if (shape.hasDoor) {
                glm::vec3 yAxis = glm::normalize(glm::cross(shape.zAxis, shape.xAxis));

                glm::mat4 doorModel = glm::mat4(1.0f);
                glm::vec3 doorSize(0.2f, 0.4f, 0.05f); 
                glm::vec3 doorOffset = yAxis * (shape.scale.y / 8.0f - doorSize.y) + 
                    shape.zAxis * (shape.scale.z / 1.5f + doorSize.z / 1.5f);   

                doorModel = glm::translate(model, doorOffset);
                doorModel = glm::scale(doorModel, doorSize);
                shader.setMat4("model", doorModel);
                glBindTexture(GL_TEXTURE_2D, doorTex);
                cube.Draw();
            }

            if (shape.hasChim) {
                glm::mat4 chimneyModel = glm::mat4(1.0f);
                glm::vec3 chimSize(0.1f, 0.4f, 0.1f);

                float xOffset = (shape.chimSide)
                    ? -shape.scale.x / 2.0f + chimSize.x / 2.0f
                    : shape.scale.x / 2.0f - chimSize.x / 2.0f;

                float yOffset = 0.5f + chimSize.y / 2.0f;

                float zOffset = -shape.scale.z * -0.1f;

                glm::vec3 chimOffset(xOffset, yOffset, zOffset);
                chimneyModel = glm::translate(model, chimOffset);
                chimneyModel = glm::scale(chimneyModel, chimSize);

                shader.setMat4("model", chimneyModel);
                glBindTexture(GL_TEXTURE_2D, chimTex);  
                cube.Draw();
            }

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}