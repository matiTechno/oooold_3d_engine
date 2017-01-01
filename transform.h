#ifndef TRANSFORM_H
#define TRANSFORM_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
public:

    Transform(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::vec3& rotation = glm::vec3(0, 0, 0), const glm::vec3& scale = glm::vec3(1, 1, 1))
    {
        m_position = position;
        m_rotation = rotation;
        m_scale = scale;
    }

    glm::mat4 getModel() const
    {
        glm::mat4 positionMatrix = glm::translate(m_position);
        glm::mat4 rotMatrixX = glm::rotate(m_rotation.x, glm::vec3(1, 0, 0));
        glm::mat4 rotMatrixY = glm::rotate(m_rotation.y, glm::vec3(0, 1, 0));
        glm::mat4 rotMatrixZ = glm::rotate(m_rotation.z, glm::vec3(0, 0, 1));
        glm::mat4 rotationMatrix = rotMatrixX * rotMatrixY * rotMatrixZ;
        glm::mat4 scaleMatrix = glm::scale(m_scale);

        return positionMatrix * rotationMatrix * scaleMatrix;
    }

    glm::vec3 &getPos_ref()
    {
        return m_position;
    }
    glm::vec3 &getRot_ref()
    {
        return m_rotation;
    }

    glm::vec3 &getScale_ref()
    {
        return m_scale;
    }

private:

    glm::vec3 m_position, m_rotation, m_scale;
};

#endif // TRANSFORM_H
