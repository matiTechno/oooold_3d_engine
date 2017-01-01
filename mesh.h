#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Vertex
{
public:
    Vertex(const glm::vec3& position, const glm::vec2& texCoord = glm::vec2(), const glm::vec3& normal = glm::vec3())
    {
        m_position = position;
        m_texCoord = texCoord;
        m_normal = normal;
    }

    glm::vec3 m_position;
    glm::vec2 m_texCoord;
    glm::vec3 m_normal;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    ~Mesh();
    Mesh()
    {
    }

    void draw();

private:
    enum
    {
        v_position,
        v_texCoord,
        v_normal,
        element,
        num_buffers
    };

    GLuint m_VAO, m_BO[num_buffers], m_numIndices;
};

#endif // MESH_H
