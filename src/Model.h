#pragma once

#include "Mesh.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

GLint TextureFromFile(const char* path, const std::string& directory);

class Model
{
public:
    Model(const std::string& path)
    {
        this->loadModel(path);
    }

    void Draw(const Shader& shader)
    {
        for(auto i = 0; i < this->meshes.size(); ++i)
        {
            this->meshes[i].Draw(shader);
        }
    }

private:
    void loadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!pScene || pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        this->directory = path.substr(0, path.find_last_of('/'));
        this->processNode(pScene->mRootNode, pScene);
    }

    void processNode(aiNode* pNode, const aiScene* pScene)
    {
        for(auto i = 0; i < pNode->mNumMeshes; ++i)
        {
            aiMesh* pMesh = pScene->mMeshes[ pNode->mMeshes[i] ];
            this->meshes.push_back( this->processMesh(pMesh, pScene) );
        }

        for(auto i = 0; i < pNode->mNumChildren; ++i)
        {
            this->processNode(pNode->mChildren[i], pScene);
        }
    }

    Mesh processMesh(aiMesh* pMesh, const aiScene* pScene)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        for (auto i = 0; i < pMesh->mNumVertices; ++i)
        {
            Vertex vertex;

            // Process vertices
			if (pMesh->mVertices)
			{
				vertex.position.x = pMesh->mVertices[i].x;
				vertex.position.y = pMesh->mVertices[i].y;
				vertex.position.z = pMesh->mVertices[i].z;
			}

			if (pMesh->mNormals)
			{
				vertex.normal.x = pMesh->mNormals[i].x;
				vertex.normal.y = pMesh->mNormals[i].y;
				vertex.normal.z = pMesh->mNormals[i].z;
			}

            if (pMesh->mTextureCoords[0])
            {
                vertex.texCoord.x = pMesh->mTextureCoords[0][i].x;
                vertex.texCoord.y = pMesh->mTextureCoords[0][i].y;
            }

            vertices.push_back(vertex);
        }

        // Process indices
        for(auto i = 0; i < pMesh->mNumFaces; ++i)
        {
            aiFace face = pMesh->mFaces[i];
            for(auto j = 0; j < face.mNumIndices; ++j)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        if(pMesh->mMaterialIndex >= 0)
        {
            aiMaterial* pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = this->loadMaterialTextures(pMaterial,
                                                aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<Texture> specularMaps = this->loadMaterialTextures(pMaterial,
                                                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> loadMaterialTextures(aiMaterial* pMat, const aiTextureType& textureType, const std::string& typeName)
    {
        std::vector<Texture> textures;
        for(auto i = 0; i < pMat->GetTextureCount(textureType); ++i)
        {
            aiString path;
            pMat->GetTexture(textureType, i, &path);
            GLboolean skip = false;
            for(GLuint j = 0; j < textures_loaded.size(); j++)
            {
                if(textures_loaded[j].path == path)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if(!skip)
            {   // If texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(path.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = path;
                textures.push_back(texture);
                this->textures_loaded.push_back(texture);  // Add to loaded textures
            }
        }
        return textures;
    }

    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;
};

inline GLint TextureFromFile(const char* path, const std::string& directory)
{
     //Generate texture ID and load texture data
    std::string filename = directory + '/' + std::string(path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height, channels;
    unsigned char* image = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    return textureID;
}
