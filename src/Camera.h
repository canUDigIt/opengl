#pragma once

// Std. Includes
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <algorithm>
#include <vector>

// GL Includes
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// An abstract camera class that processes input and calculates
// the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
  // Camera Attributes
  glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
  GLfloat Yaw = 0.0f;
  GLfloat Pitch = 0.0f;
  GLfloat Distance = 45.0f;

  // Camera options
  const GLfloat SPEED = 3.0f;
  const GLfloat SENSITIVTY = 0.25f;

  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f))
  {
    this->Position = position;
    this->WorldUp = up;
    this->Center = center;
    this->updateCameraVectors();
  }
  // Constructor with scalar values
  Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY,
         GLfloat upZ, GLfloat centerX, GLfloat centerY, GLfloat centerZ)
  {
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Center = glm::vec3(centerX, centerY, centerZ);
    this->updateCameraVectors();
  }

  // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix() {
    return glm::lookAt(this->Position, this->Center, this->WorldUp);
  }

  // Processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
    GLfloat velocity = SPEED * deltaTime;
    if (direction == FORWARD)
      this->Position += this->Front * velocity;
    if (direction == BACKWARD)
      this->Position -= this->Front * velocity;
    if (direction == LEFT)
      this->Position -= this->Right * velocity;
    if (direction == RIGHT)
      this->Position += this->Right * velocity;
  }

  // Processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset,
                            GLboolean constrainPitch = true) {
    this->Yaw += xoffset * SENSITIVTY;
    this->Pitch += yoffset * SENSITIVTY;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      std::min(std::max(this->Pitch, -89.0f), 89.0f);
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
  }

  // Processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void ProcessMouseScroll(GLfloat yoffset) {
    this->Distance -= yoffset;
    std::min(std::max(this->Pitch, 1.0f), 45.0f);
  }

private:
  // Calculates the front vector from the Camera's (updated) Eular Angles
  void updateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 pos;
    pos.x = cos(glm::radians(this->Yaw)) * sin(glm::radians(this->Pitch));
    pos.y = cos(glm::radians(this->Pitch));
    pos.z = sin(glm::radians(this->Yaw)) * sin(glm::radians(this->Pitch));
    this->Position = glm::normalize(pos + this->Center);

    this->Front = glm::normalize(this->Center - this->Position);

    // Also re-calculate the Right and Up vector
    // Normalize the vectors, because their length gets
    // closer to 0 the more you look up or down which
    // results in slower movement.
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
  }
};
