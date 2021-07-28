#pragma once

// Std. Includes
#include <algorithm>
#include <cmath>
#include <vector>

// GL Includes
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct ArcBallCamera {
  glm::vec3 lookat = glm::vec3(0.0f, 0.0f, 0.0f);
  float pitch_radians = 0.0f;
  float swivel_radians = 0.0f;
  float distance = 3.0f;
};

inline glm::mat4 convert_to_view_matrix(const ArcBallCamera& camera) {
    glm::vec3 up {0.0f, 1.0f, 0.0f};
    glm::vec3 right {1.0f, 0.0f, 0.0f};
    glm::vec3 eye {0.0f, 0.0f, camera.distance};

    glm::mat4 rotateXY = glm::rotate(glm::mat4 {1.0f}, camera.swivel_radians, up);
    glm::vec4 rotatedRight = rotateXY * glm::vec4{right, 1.0f};
    glm::mat4 rotateXYZ = glm::rotate(rotateXY, camera.pitch_radians, glm::vec3{rotatedRight});

    eye = rotateXYZ * glm::vec4{eye, 1.0f};
    return glm::lookAt(eye, camera.lookat, up);
}
