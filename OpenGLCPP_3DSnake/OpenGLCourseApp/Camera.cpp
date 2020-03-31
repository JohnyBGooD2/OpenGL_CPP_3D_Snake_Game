#include "Camera.h"


Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();

}

// SDL keys reference https://wiki.libsdl.org/SDLKeycodeLookup , https://wiki.libsdl.org/SDL_Keycode

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime; //moveSpeed * deltaTime;

	if (keys[SDLK_w])
	{
		position += front * velocity;
	}

	if (keys[SDLK_s])
	{
		position -= front * velocity;
	}

	if (keys[SDLK_a])
	{
		position -= right * velocity;
	}

	if (keys[SDLK_d])
	{
		position += right * velocity;
	}

	if (keys[SDLK_SPACE])
	{
		position += worldUp * velocity;
	}
	              
	// 297:LALT   

	if (keys[297]) // Left alt code from first link minus 1073742050 , see Window.cpp for more
	{
		position -= worldUp * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}


glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}



void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
