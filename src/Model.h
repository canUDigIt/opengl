#pragma once

#include "Mesh.h"

#include <QImage>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

uint32_t TextureFromFile(QOpenGLExtraFunctions* gl, const std::string& path, const std::string& directory);

class Model
{
public:
    Model() = default;

    void load(QOpenGLExtraFunctions* gl, const std::string& path)
    {
        loadModel(gl, path);
    }

    void draw(QOpenGLExtraFunctions* gl, Shader& program)
    {
        for(auto mesh : meshes)
        {
            mesh.draw(gl, program);
        }
    }

private:
    void loadModel(QOpenGLExtraFunctions* gl, const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!pScene || pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));
        processNode(gl, pScene->mRootNode, pScene);
    }

    void processNode(QOpenGLExtraFunctions* gl, aiNode* pNode, const aiScene* pScene)
    {
        for(auto i = 0; i < pNode->mNumMeshes; ++i)
        {
            aiMesh* pMesh = pScene->mMeshes[ pNode->mMeshes[i] ];
            meshes.push_back( processMesh(gl, pMesh, pScene) );
        }

        for(auto i = 0; i < pNode->mNumChildren; ++i)
        {
            processNode(gl, pNode->mChildren[i], pScene);
        }
    }

    Mesh processMesh(QOpenGLExtraFunctions* gl, aiMesh* pMesh, const aiScene* pScene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Texture> textures;

        for (auto i = 0; i < pMesh->mNumVertices; ++i)
        {
            Vertex vertex;

            // Process vertices
			if (pMesh->mVertices)
			{
				vertex.position.setX(pMesh->mVertices[i].x);
				vertex.position.setY(pMesh->mVertices[i].y);
				vertex.position.setZ(pMesh->mVertices[i].z);
			}

			if (pMesh->mNormals)
			{
				vertex.normal.setX(pMesh->mNormals[i].x);
				vertex.normal.setY(pMesh->mNormals[i].y);
				vertex.normal.setZ(pMesh->mNormals[i].z);
			}

            if (pMesh->mTextureCoords[0])
            {
				vertex.texCoord.setX(pMesh->mTextureCoords[0][i].x);
				vertex.texCoord.setY(pMesh->mTextureCoords[0][i].y);
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

            std::vector<Texture> diffuseMaps = loadMaterialTextures(gl, pMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Texture> specularMaps = loadMaterialTextures(gl, pMaterial, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        Mesh mesh {};
        mesh.load(gl, vertices, indices, textures);
        return mesh;
    }

    std::vector<Texture> loadMaterialTextures(QOpenGLExtraFunctions* gl, aiMaterial* pMat, const aiTextureType& textureType, const std::string& typeName)
    {
        std::vector<Texture> textures;
        for(auto i = 0; i < pMat->GetTextureCount(textureType); ++i)
        {
            aiString assimpPath;
            pMat->GetTexture(textureType, i, &assimpPath);
            std::string path {assimpPath.C_Str()};
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
                texture.id = TextureFromFile(gl, path, directory);
                texture.type = typeName;
                texture.path = path;
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // Add to loaded textures
            }
        }
        return textures;
    }

    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;
};
