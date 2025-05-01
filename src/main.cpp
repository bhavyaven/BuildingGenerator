#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <random>

#include "shape.hpp"
#include "rule.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"

// Constants
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
        float yoffset = lastY - ypos; // reversed since y-coordinates go bottom to top

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

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Grammar application
void applyRule(const Rule& rule, Shape& parent, std::vector<Shape>& output) {
    float randVal = static_cast<float>(rand()) / RAND_MAX;
    if (randVal <= rule.probability)
        rule.func(parent, output);
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

    Mesh cube = createCubeMesh();
    Mesh roof = createRoofMesh();

    // Initial shape
    Material mat = { glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(0.2f, 0.2f, 0.2f) };
    Shape base('H', glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), mat,
        glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), false);

    std::vector<Shape> shapes;
    shapes.push_back(base);

    // First grammar rule
    std::vector<Rule> rules;
    rules.emplace_back(1.0f, [](Shape& parent, std::vector<Shape>& output) {
        Shape front = parent;
        front.scale.z *= 0.5f;
        front.position += parent.zAxis * (parent.scale.z * 0.25f);
        output.push_back(parent); // keep base
        output.push_back(front); // add front
        });

    std::vector<Shape> newShapes;
    for (Shape& s : shapes) {
        for (Rule& r : rules) applyRule(r, s, newShapes);
    }
    shapes = newShapes;

    shader.use();
    shader.setVec3("roofColor", mat.roofColor);
    shader.setVec3("wallColor", mat.wallColor);


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
            model = glm::translate(model, shape.position);
            //model = glm::rotate(model, shape.rotation.y, glm::vec3(0, 1, 0));
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
            model = glm::scale(model, shape.scale);
            shader.setVec3("color", shape.material.wallColor);
            shader.setMat4("model", model);
            cube.Draw();

            glm::mat4 roofModel = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
            shader.setVec3("color", shape.material.roofColor);
            shader.setMat4("model", roofModel);
            roof.Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}