#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "shader.h"
#include <vector>
#include "modtexmesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


class Model
{

public:

    Model(const std::string& fileName)
    {
        loadModel(fileName);
    }

    ~Model();

    void draw(const Shader* shader, float shininess)
    {
        for(GLuint i = 0; i < m_meshes.size(); i++)
            m_meshes[i].draw(shader, shininess);
    }
    void draw()
    {
        for(GLuint i = 0; i < m_meshes.size(); i++)
            m_meshes[i].draw();
    }

private:

    std::vector<ModTexMesh> m_meshes;
    std::string m_directory;
    std::vector<ModTexture> m_loaded_textures;

    void loadModel(const std::string& full_fileName);

    void processNode(aiNode* node, const aiScene* scene);

    ModTexMesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<ModTexture> loadTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);

    GLuint stbiLoadTexture(const std::string& fileName, const std::string& m_directory);
};

#endif // MODEL_H
