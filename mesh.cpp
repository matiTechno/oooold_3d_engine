#include "mesh.h"
#include <vector>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
    m_numIndices = indices.size();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(num_buffers, m_BO);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    for(GLuint i = 0; i < vertices.size(); i++)
    {
        positions.push_back(vertices[i].m_position);
        texCoords.push_back(vertices[i].m_texCoord);
        normals.push_back(vertices[i].m_normal);
    }

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_BO[v_position]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_BO[v_texCoord]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_BO[v_normal]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BO[element]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(num_buffers, m_BO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::draw()
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
