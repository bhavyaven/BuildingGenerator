#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// from learn open gl camera.h class

enum CamMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float moveSpeed;
	float mouseSense;
	float zoom;

	Camera(glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f),
		float Yaw = YAW, float Pitch = PITCH)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)),
		moveSpeed(SPEED), mouseSense(SENSITIVITY), zoom(ZOOM) {
		pos = Pos;
		worldUp = Up;
		yaw = Yaw;
		pitch = Pitch;
		updateCameraVectors();
	}

	Camera(float xPos, float yPos, float zPos,
		float xUp, float yUp, float zUp,
		float Yaw, float Pitch)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)),
		moveSpeed(SPEED), mouseSense(SENSITIVITY), zoom(ZOOM) {
		pos = glm::vec3(xPos, yPos, zPos);
		worldUp = glm::vec3(xUp, yUp, zUp);
		yaw = Yaw;
		pitch = Pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(pos, pos + front, up);
	}

	void ProcessKeyboard(CamMovement dir, float deltaTime) {
		float velocity = moveSpeed * deltaTime;
		if (dir == FORWARD) {
			pos += front * velocity;
		}

		if (dir == BACKWARD) {
			pos -= front * velocity;
		}

		if (dir == LEFT) {
			pos -= right * velocity;
		}

		if (dir == RIGHT) {
			pos += right * velocity;
		}

	}

	void ProcessMouse(float xOffset, float yOffset, GLboolean constrainPitch = true) {
		xOffset *= mouseSense;
		yOffset *= mouseSense;

		yaw += xOffset;
		pitch += yOffset;

		if (constrainPitch) {
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}

			if (pitch < 89.0f) {
				pitch = -89.0f;
			}
		}
		updateCameraVectors();
	}

	void ProcessScroll(float yOffset) {
		zoom -= (float)yOffset;
		if (zoom < 1.0f) {
			zoom = 1.0f;
		}

		if (zoom > 45.0f) {
			zoom = 45.0f;
		}
	}

private:
	void updateCameraVectors() {
		glm::vec3 f;
		f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		f.y = sin(glm::radians(pitch));
		f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(f);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};
#endif