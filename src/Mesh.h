#pragma once

#include "Shader.h"
#include "Camera.h"

#include <stdint.h>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

struct IndexedTriangleList {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

struct Texture
{
    GLuint id;
    std::string type;
    std::string path;
};

struct Material {
    Shader shader;
    std::vector<Texture> textures;
};

class Mesh
{
public:
    IndexedTriangleList triList;
    Material material;

    glm::mat4 modelMatrix{1.0};

    Mesh(const IndexedTriangleList& list, const Material& mat)
        :triList(list)
        , material(mat)
    {
    }

    void CreateGraphicResources() {
      GLuint VBO;
      GLuint EBO;
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, triList.vertices.size() * sizeof(Vertex),
                   triList.vertices.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   triList.indices.size() * sizeof(uint32_t), triList.indices.data(),
                   GL_STATIC_DRAW);

      // Vertex positions
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (GLvoid *)0);

      // Vertex normals
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (GLvoid *)offsetof(Vertex, normal));

      // Vertex texcoords
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (GLvoid *)offsetof(Vertex, texCoord));

      glBindVertexArray(0);
    }

    void Draw(const Camera& camera)
    {
        material.shader.Use();

        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        for(auto i = 0; i < material.textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string name = material.textures[i].type;
            std::string number;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            glUniform1f(glGetUniformLocation(material.shader.Program, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, material.textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // Transformation matrices
        glm::mat4 projection = camera.GetPerspectiveMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(material.shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(material.shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glUniformMatrix4fv(glGetUniformLocation(material.shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, triList.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Cleaning up texture state
        for(auto i = 0; i < material.textures.size(); ++i){
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

private:
    GLuint VAO;
};
