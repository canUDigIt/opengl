#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position{0.0f, 0.0f, 0.0f};
	glm::vec3 Front{0.0f, 0.0f, -1.0f};
	glm::vec3 Up{0.0f, 1.0f, 0.0f};
	glm::vec3 Right{1.0f, 0.0f, 0.0f};
	glm::vec3 WorldUp{0.0f, 1.0f, 0.0f};
	// Eular Angles
	GLfloat Yaw = -90.0f;
	GLfloat Pitch = 0.0f;
	// Camera options
	GLfloat MovementSpeed = 3.0f;
	GLfloat MouseSensitivity = 0.25f;
	GLfloat Zoom = 45.0f;
    int Width = 0;
    int Height = 0;
    float NearZ = 0.f;
    float FarZ = 1000.f;

	Camera() { updateCameraVectors(); }

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = -90.0f,
           GLfloat pitch = 0.0f)
        : Position(position), Up(up), Yaw(yaw), Pitch(pitch) {
      updateCameraVectors();
    }

        // Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
        : Position(posX, posY, posZ), Up(upX, upY, upZ), Yaw(yaw), Pitch(pitch)
	{
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

    glm::mat4 GetPerspectiveMatrix()
    {
        return glm::perspective(Zoom, static_cast<float>(Width)/static_cast<float>(Height), NearZ, FarZ);
    }

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime)
	{
		GLfloat velocity = MovementSpeed * deltaTime;
		if (direction == CameraMovement::FORWARD)
			Position += Front * velocity;
		if (direction == CameraMovement::BACKWARD)
			Position -= Front * velocity;
		if (direction == CameraMovement::LEFT)
			Position -= Right * velocity;
		if (direction == CameraMovement::RIGHT)
			Position += Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
