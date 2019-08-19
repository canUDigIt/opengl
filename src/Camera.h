#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

// Std. Includes
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVTY = 0.25f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	QVector3D Position {0.0f, 0.0f, 0.0f};
	QVector3D Front {0.0f, 0.0f, -1.0f};
	QVector3D Up {0.0f, 1.0f, 0.0f};
	QVector3D Right {1.0, 0.0f, 0.0f};
	QVector3D WorldUp {0.0f, 1.0f, 0.0f};
	// Eular Angles
	float Yaw = YAW;
	float Pitch = PITCH;
	// Camera options
	float MovementSpeed = SPEED;
	float MouseSensitivity = SENSITIVTY;
	float Zoom = ZOOM;

    Camera() = default;
    ~Camera() = default;

	// Constructor with vectors
	Camera(QVector3D position, QVector3D up)
	{
		Position = position;
		WorldUp = up;
		updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	{
		Position = QVector3D(posX, posY, posZ);
		WorldUp = QVector3D(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	QMatrix4x4 GetViewMatrix()
	{
        QMatrix4x4 view;
        view.lookAt(Position, Position + Front, Up);
		return view;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
	void ProcessMouseScroll(float yoffset)
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
		Front.setX(cos(qDegreesToRadians(Yaw)) * cos(qDegreesToRadians(Pitch)));
        Front.setY(sin(qDegreesToRadians(Pitch)));
        Front.setZ(sin(qDegreesToRadians(Yaw)) * cos(qDegreesToRadians(Pitch)));
        Front.normalize();

		// Also re-calculate the Right and Up vector
		Right = QVector3D::crossProduct(Front, WorldUp).normalized();

		Up = QVector3D::crossProduct(Right, Front).normalized();
	}
};
