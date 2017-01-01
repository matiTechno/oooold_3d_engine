#include "modtexmesh.h"
#include <sstream>

ModTexMesh::ModTexMesh(const std::vector<ModVertex>& vertices, const std::vector<GLuint>& indices, const std::vector<ModTexture>& textures)
{
    m_num_indices = indices.size();
    m_textures = textures;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ModVertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModVertex), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModVertex), reinterpret_cast<GLvoid *>(offsetof(ModVertex, texCoord)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ModVertex), reinterpret_cast<GLvoid *>(offsetof(ModVertex, normal)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void ModTexMesh::draw(const Shader* shader, float shininess)
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);

    shader->bind();

    GLuint diffuseNr = 0;
    GLuint specularNr = 0;

    for(GLuint i = 0; i < m_textures.size(); i++)
    {
        std::stringstream stream;
        if(m_textures[i].type == "mat_tex_diffuse")
            stream << diffuseNr++;
        else if(m_textures[i].type == "mat_tex_specular")
            stream << specularNr++;

        glUniform1i(glGetUniformLocation(shader->getProgram_ref(), (m_textures[i].type + stream.str()).c_str()), i + 1);
        glActiveTexture(GL_TEXTURE0 + i + 1);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
    glUniform1f(glGetUniformLocation(shader->getProgram_ref(), "mat_shininess"), shininess);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ModTexMesh::draw()
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
