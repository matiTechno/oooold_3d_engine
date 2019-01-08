#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:

    Camera(const glm::vec3& position, float fov, float aspect, float zNear, float zFar)
    {
        m_position = position;
        m_projection = glm::perspective(fov, aspect, zNear, zFar);
        m_forward = glm::vec3(0, 0, -1);
        m_up = glm::vec3(0, 1, 0);
    }

    glm::mat4 getProjection() const
    {
        return m_projection;
    }

    glm::mat4 getView() const
    {
        return glm::lookAt(m_position, m_position + m_forward, m_up);
    }
    glm::vec3 getPosition() const
    {
        return m_position;
    }
    // movement

    void moveForward(float coeff)
    {
        m_position += m_forward * coeff;
    }
    void moveRight(float coeff)
    {
        m_position += glm::cross(m_forward, m_up) * coeff;
    }
    void rotateX(float angle)
    {
        glm::vec3 right = glm::cross(m_forward, m_up);
        m_forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(m_forward, 0)));
        m_up = glm::cross(right, m_forward);
    }
    void rotateY(float angle)
    {
        glm::vec3 UP(0, 1, 0);
        m_forward = glm::vec3(glm::normalize(glm::rotate(angle, UP)  * glm::vec4(m_forward, 0)));
        m_up = glm::vec3(glm::normalize(glm::rotate(angle, UP) * glm::vec4(m_up, 0)));
    }

private:
    glm::vec3 m_position, m_forward, m_up;
    glm::mat4 m_projection;

};

#endif // CAMERA_H
