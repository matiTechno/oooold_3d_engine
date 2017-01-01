#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>
#include <vector>
#include "shader.h"
#include "transform.h"

class SunLight // ################### S U N   L I G H T
{
public:
    SunLight(Transform* transform, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
    {
        m_transform = transform;
        m_ambient = ambient;
        m_diffuse = diffuse;
        m_specular = specular;
    }

    glm::vec3 m_ambient, m_diffuse, m_specular;
    Transform* m_transform;
};

class PointLight // ################### P O I N T   L I G H T
{
public:
    PointLight(Transform* transform, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float a, float b, float c)
    {
        m_transform = transform;
        m_ambient = ambient;
        m_diffuse = diffuse;
        m_specular = specular;
        m_a = a;
        m_b = b;
        m_c = c;
    }

    glm::vec3 m_ambient, m_diffuse, m_specular;
    float m_a, m_b, m_c;
    Transform* m_transform;
};

class SpotLight // ################### S P O T   L I G H T
{
public:
    SpotLight(const glm::vec3& direction, Transform* transform, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
              float a, float b, float c, float innerAngle, float outerAngle)
    {
        m_direction = direction;
        m_transform = transform;
        m_ambient = ambient;
        m_diffuse = diffuse;
        m_specular = specular;
        m_a = a;
        m_b = b;
        m_c = c;
        m_innerAngle = innerAngle;
        m_outerAngle = outerAngle;
    }

    glm::vec3 m_direction, m_ambient, m_diffuse, m_specular;
    float m_a, m_b, m_c, m_innerAngle, m_outerAngle;
    Transform* m_transform;
};

#endif // LIGHTS_H
