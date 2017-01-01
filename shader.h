#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include "camera.h"
#include "transform.h"
#include "vector"
#include "lights.h"


class Shader
{

public:

    Shader(const std::string& fileName_no_suffix, bool geometryShader = false);

    std::string load(const std::string& fileName);

    GLuint compile(const std::string& text, GLenum shaderType);

    void getError(GLuint shader, GLenum flag, bool isProgram, const std::string& message);

    void update(const Camera* camera = 0, const Transform* transform = 0);

    ~Shader();

    const GLuint& getProgram_ref() const
    {
        return m_program;
    }

    void bind() const
    {
        glUseProgram(m_program);
    }

    // additional functions
    void setMat(float shininess, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
    void setMat(float shininess);
    void setLightScene(const std::vector<SunLight>* sunLights, const std::vector<PointLight>* pointLights = 0, const std::vector<SpotLight>* spotLights = 0);

    void setSimpleColor(const glm::vec3& color)
    {
        bind();
        glUniform3f(glGetUniformLocation(m_program, "simpleColor"), color.x, color.y, color.z);
    }

private:

    GLuint m_vertexShader, m_geometryShader, m_fragmentShader, m_program;

};

#endif // SHADER_H
