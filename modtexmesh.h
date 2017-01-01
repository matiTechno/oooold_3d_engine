#ifndef MODTEXMESH_H
#define MODTEXMESH_H

#include <GL/glew.h>
#include <string>
#include <assimp/types.h>
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"



class ModVertex
{
public:
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
};



class ModTexture
{
public:
    GLuint id;
    std::string type;
    aiString fileName;
};



class ModTexMesh
{

public:

    ModTexMesh(const std::vector<ModVertex>& vertices, const std::vector<GLuint>& indices, const std::vector<ModTexture>& textures);
    void draw(const Shader* shader, float shininess);
    void draw();


    GLuint m_VAO, m_EBO, m_VBO, m_num_indices; // potrzebne do destruktora w klasie model

private:

    std::vector<ModTexture> m_textures;
};

#endif // MODTEXMESH_H
