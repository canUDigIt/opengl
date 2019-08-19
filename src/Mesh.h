#pragma once

#include "Shader.h"

#include <QOpenGLExtraFunctions>

#include <string>
#include <vector>

#include <assimp/types.h>

struct Vertex
{
    QVector3D position;
    QVector3D normal;
    QVector3D texCoord;
};

struct Texture
{
    uint32_t id = 0;
    std::string type = "";
    std::string path = "";
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh() = default;
    ~Mesh() = default;

    void load(QOpenGLExtraFunctions* gl, const std::vector<Vertex>& v, const std::vector<uint32_t>& i, const std::vector<Texture>& t)
    {
        vertices = v;
        indices = i;
        textures = t;

        setupMesh(gl);
    }

    void draw(QOpenGLExtraFunctions* gl, Shader& program)
    {
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        for(auto i = 0; i < textures.size(); ++i) {
            gl->glActiveTexture(GL_TEXTURE0 + i);

            std::string name = textures[i].type;
            std::string number;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            program.setInt((name + number).c_str(), i);
            gl->glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        gl->glActiveTexture(GL_TEXTURE0);

        gl->glBindVertexArray(VAO);
        gl->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        gl->glBindVertexArray(0);

        // Cleaning up texture state
        for(auto i = 0; i < textures.size(); ++i){
            gl->glActiveTexture(GL_TEXTURE0 + i);
            gl->glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

private:
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;

    void setupMesh(QOpenGLExtraFunctions* gl)
    {
        gl->glGenVertexArrays(1, &VAO);
        gl->glGenBuffers(1, &VBO);
        gl->glGenBuffers(1, &EBO);

        gl->glBindVertexArray(VAO);

        gl->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        gl->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        //Vertex positions
        gl->glEnableVertexAttribArray(0);
        gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

        // Vertex normals
        gl->glEnableVertexAttribArray(1);
        gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

        // Vertex texcoords
        gl->glEnableVertexAttribArray(2);
        gl->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

        gl->glBindVertexArray(0);
    }
};
