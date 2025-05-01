#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "interpreter.h"
#include "camera.h"
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

std::stringstream preprocessStream(std::istream& istr);
std::string getNextLine(std::istream& istr);
std::string trim(const std::string& line);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


void Interpreter::loadGrammar(const std::string& fileName) {
	std::cout << "enter grammar func" << std::endl;
	std::ifstream file(fileName);
	std::cout << fileName << std::endl;
	std::string line;
	Rule currRule;
	std::string currRuleName;
	bool insideRule = false;

	std::stringstream ss = preprocessStream(file);

	while (std::getline(ss, line)) {
		std::cout << line << std::endl;
		size_t comment = line.find("//");
		if (comment != std::string::npos) {
			line = line.substr(0, comment);
		}

		if (line.empty()) {
			continue;
		}

		if (line.find("param") == 0) {
			size_t equal = line.find('=');
			if (equal == std::string::npos || equal <= 6) {
				std::cerr << "Malformed param line: " << line << std::endl;
				continue;
			}
			std::string name = line.substr(6, equal - 6);
			size_t semi = line.find(";");
			std::string val = line.substr(equal + 1, semi - equal - 1);
			params[name] = val;
		}
		else if (line.find("const") == 0) {
			continue;
		}
		else if (line.find("rule") == 0) {
			size_t start = line.find(' ') + 1;
			size_t end = line.find('=');
			currRuleName = line.substr(start, end - start);
			std::cout << "rule name: " << currRuleName << std::endl;
			insideRule = true;
			currRule.commands.clear();
		}
		else if (line.find('}') != std::string::npos) {
			rules[currRuleName] = currRule;
			insideRule = false;
		}
		else if (insideRule) {
			size_t paren = line.find('(');
			size_t endParen = line.find(')');
			if (paren != std::string::npos && endParen != std::string::npos) {
				Command cmd;
				cmd.name = line.substr(0, paren);
				std::string args = line.substr(paren + 1, endParen - paren - 1);

				std::stringstream ss(args);
				std::string arg;
				while (std::getline(ss, arg, ',')) {
					cmd.args.push_back(arg);
				}

				std::cout << "cmd 1: " << cmd.name << std::endl;
				std::cout << "args: " << args << std::endl;

				currRule.commands.push_back(cmd);
			}
			else {
				Command cmd;
				cmd.name = line;
				std::cout << "cmd 2: " << cmd.name << std::endl;

				currRule.commands.push_back(cmd);
			}
		}
	}
}

// Remove empty lines, comments, and trim leading and trailing whitespace
std::stringstream preprocessStream(std::istream& istr) {
	istr.exceptions(istr.badbit | istr.failbit);
	std::stringstream ss;

	try {
		while (true) {
			std::string line = getNextLine(istr);
			ss << line << std::endl;	// Add newline after each line
		}								// Stream always ends with a newline

	}
	catch (const std::exception& e) {
		if (!istr.eof()) throw e;
	}

	return ss;
}

// Reads lines from istream, stripping whitespace and comments,
// until it finds a nonempty line
std::string getNextLine(std::istream& istr) {
	const std::string comment = "//";
	std::string line = "";
	while (line == "") {
		std::getline(istr, line);
		// Skip comments and empty lines
		auto found = line.find(comment);
		if (found != std::string::npos)
			line = line.substr(0, found);
		line = trim(line);
	}
	return line;
}

// Trim leading and trailing whitespace from a line
std::string trim(const std::string& line) {
	const std::string whitespace = " \t\r\n";
	auto first = line.find_first_not_of(whitespace);
	if (first == std::string::npos)
		return "";
	auto last = line.find_last_not_of(whitespace);
	auto range = last - first + 1;
	return line.substr(first, range);
}


void Interpreter::executeRule(const std::string& ruleName, Shape shape) {
	if (rules.find(ruleName) == rules.end()) {
		return;
	}

	const Rule& rule = rules[ruleName];
	Shape currShape = shape;
	/*currShape.pos = glm::vec3(0.0f);
	currShape.size = glm::vec3(1.0f);
	currShape.color = glm::vec3(1.0f);*/

	for (const auto& cmd : rule.commands) {
		if (cmd.name == "size") {
			currShape.size = glm::vec3(
				std::stof(cmd.args[0]),
				std::stof(cmd.args[1]),
				std::stof(cmd.args[2])
			);
		}
		else if (cmd.name == "quad") {
			currShape.type = "quad";
			shapes.push_back(currShape);
		}
		else if (rules.find(cmd.name) != rules.end()) {
			executeRule(cmd.name, currShape);
		}
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// GLFW framebuffer size callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	float posX = static_cast<float>(xPos);
	float posY = static_cast<float>(yPos);

	if (firstMouse) {
		lastX = posX;
		lastY = posY;
		firstMouse = false;
	}

	float xOffset = posX - lastX;
	float yOffset = posY - lastY;

	lastX = posX;
	lastY = posY;

	camera.ProcessMouse(xOffset, yOffset);
}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessScroll(static_cast<float>(yOffset));
}

void Interpreter::initOpenGL() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Building Generator", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);

	loadShaders();
	setupCube();
}

void Interpreter::run() {
	GLFWwindow* window = glfwGetCurrentContext();

	executeRule("Axiom");
	std::cout << "Shapes: " << shapes.size() << std::endl;


	while (!glfwWindowShouldClose(window)) {
		float currFrame = static_cast<float>(glfwGetTime());
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;

		processInput(window);

		glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));

		for (const auto& shape : shapes) {
			drawShape(shape);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Interpreter::setupCube() {
	float vertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		// Front face
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		// Left face
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		// Right face
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,

		 // Bottom face
		 -0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f,  0.5f,
		  0.5f, -0.5f,  0.5f,
		 -0.5f, -0.5f,  0.5f,
		 -0.5f, -0.5f, -0.5f,

		 // Top face
		 -0.5f,  0.5f, -0.5f,
		  0.5f,  0.5f,  0.5f,
		  0.5f,  0.5f, -0.5f,
		  0.5f,  0.5f,  0.5f,
		 -0.5f,  0.5f, -0.5f,
		 -0.5f,  0.5f,  0.5f,
	};

	std::cout << "cube" << std::endl;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::cout << "end cube" << std::endl;
}

void Interpreter::loadShaders() {
	std::ifstream vFile("../../shaders/v.glsl");
	std::ifstream fFile("../../shaders/f.glsl");
	if (!vFile.is_open() || !fFile.is_open()) {
		std::cerr << "shader problem" << std::endl;
		return;
	}
	std::stringstream vSS, fSS;
	vSS << vFile.rdbuf();
	fSS << fFile.rdbuf();

	std::string v = vSS.str();
	std::string f = fSS.str();

	const char* vCode = v.c_str();
	const char* fCode = f.c_str();

	unsigned int vertex, fragment;
	int success;
	char info[1024];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, info);
		std::cout << "vertex error: " << info << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, info);
		std::cout << "fragment error: " << info << std::endl;
	}

	shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 1024, NULL, info);
		std::cout << "shader error: " << info << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Interpreter::drawShape(const Shape& shape) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, shape.pos);
	model = glm::scale(model, shape.size);

	std::cout << "Drawing shape at: " << shape.pos.x << ", " << shape.pos.y << ", " << shape.pos.z << std::endl;


	unsigned int modelLoc = glGetUniformLocation(shader, "model");
	unsigned int colorLoc = glGetUniformLocation(shader, "ourColor");
	unsigned int projLoc = glGetUniformLocation(shader, "projection");
	unsigned int viewLoc = glGetUniformLocation(shader, "view");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(colorLoc, 1, &shape.color[0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
