#include "shader.h"
#include <iostream>
#include <sstream>
#include <fstream>

Shader::Shader(const std::string& fileName_no_suffix, bool geometryShader)
{
    m_program = glCreateProgram();
    m_vertexShader = compile(load(fileName_no_suffix + ".vs"), GL_VERTEX_SHADER);
    getError(m_vertexShader, GL_COMPILE_STATUS, false, "could not compile " + fileName_no_suffix + ".vs\n");
    m_fragmentShader = compile(load(fileName_no_suffix + ".fs"), GL_FRAGMENT_SHADER);
    getError(m_fragmentShader, GL_COMPILE_STATUS, false, "could not compile " + fileName_no_suffix + ".fs\n");

    glAttachShader(m_program, m_vertexShader);
    glAttachShader(m_program, m_fragmentShader);

    if(geometryShader)
    {
        m_geometryShader = compile(load(fileName_no_suffix + ".gs"), GL_GEOMETRY_SHADER);
        getError(m_geometryShader, GL_COMPILE_STATUS, false, "could not compile " + fileName_no_suffix + ".gs\n");
        glAttachShader(m_program, m_geometryShader);
    }

    glLinkProgram(m_program);
    getError(m_program, GL_LINK_STATUS, true, "could not link program\n");

    glDetachShader(m_program, m_vertexShader);
    glDeleteShader(m_vertexShader);
    glDetachShader(m_program, m_fragmentShader);
    glDeleteShader(m_fragmentShader);

    if(geometryShader)
    {
        glDetachShader(m_program, m_geometryShader);
        glDeleteShader(m_geometryShader);
    }
}

std::string Shader::load(const std::string& fileName)
{
    std::string output;
    std::ifstream file(fileName);
    if(file)
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        output = buffer.str();
    }
    else
        std::cout << "could not open " << fileName << std::endl;
    return output;
}

GLuint Shader::compile(const std::string& text, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    const GLchar* strings[1];
    GLint lengths[1];

    strings[0] = text.c_str();
    lengths[0] = text.length();

    glShaderSource(shader, 1, strings, lengths);
    glCompileShader(shader);

    return shader;
}

void Shader::getError(GLuint shader, GLenum flag, bool isProgram, const std::string& message)
{
    GLint success = 0;
    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        GLint maxLength = 0;
        if(isProgram)
        {
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            GLchar errorLog[maxLength];
            glGetProgramInfoLog(shader, maxLength, 0, &errorLog[0]);
            std::cout << message << errorLog << std::endl;
        }
        else
        {
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            GLchar errorLog[maxLength];
            glGetShaderInfoLog(shader, maxLength, 0, &errorLog[0]);
            std::cout << message << errorLog << std::endl;
        }
    }
}

void Shader::update(const Camera* camera, const Transform* transform)
{
    glUseProgram(m_program);

    if(camera)
    {
        glm::mat4 projection = camera->getProjection();
        glm::mat4 view = camera->getView();
        glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, &view[0][0]);
        glUniform3f(glGetUniformLocation(m_program, "camPosition"), camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
    }

    glm::mat4 model;
    if(transform)
        model = transform->getModel();
    else
        model = Transform().getModel();
    glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, &model[0][0]);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::setMat(float shininess, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
{
    glUseProgram(m_program);
    glUniform3f(glGetUniformLocation(m_program, "mat_ambient"), ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(m_program, "mat_diffuse"), diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(m_program, "mat_specular"), specular.x, specular.y, specular.z);
    glUniform1f(glGetUniformLocation(m_program, "mat_shininess"), shininess);
}

void Shader::setMat(float shininess)
{
    glUseProgram(m_program);
    glUniform1i(glGetUniformLocation(m_program, "mat_tex_diffuse0"), 1);
    glUniform1i(glGetUniformLocation(m_program, "mat_tex_specular0"), 2);
    glUniform1f(glGetUniformLocation(m_program, "mat_shininess"), shininess);
}

void Shader::setLightScene(const std::vector<SunLight>* sunLights, const std::vector<PointLight>* pointLights, const std::vector<SpotLight>* spotLights)
{
    glUseProgram(m_program);

    if(sunLights)
    {
        int i = 0;
        for(std::vector<SunLight>::const_iterator it = sunLights->begin(); it != sunLights->end(); ++it)
        {
            std::string string = "sunLights[" + std::to_string(i) + "]";
            glUniform3f(glGetUniformLocation(m_program, (string + ".direction").c_str()), -it->m_transform->getPos_ref().x, -it->m_transform->getPos_ref().y, -it->m_transform->getPos_ref().z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".ambient").c_str()), it->m_ambient.x, it->m_ambient.y, it->m_ambient.z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".diffuse").c_str()), it->m_diffuse.x, it->m_diffuse.y, it->m_diffuse.z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".specular").c_str()), it->m_specular.x, it->m_specular.y, it->m_specular.z);
            i++;
        }
    }
    if(pointLights)
    {
        int i = 0;
        for(std::vector<PointLight>::const_iterator it = pointLights->begin(); it != pointLights->end(); ++it)
        {
            std::string string = "pointLights[" + std::to_string(i) + "]";
            glUniform3f(glGetUniformLocation(m_program, (string + ".position").c_str()), it->m_transform->getPos_ref().x, it->m_transform->getPos_ref().y, it->m_transform->getPos_ref().z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".ambient").c_str()), it->m_ambient.x, it->m_ambient.y, it->m_ambient.z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".diffuse").c_str()), it->m_diffuse.x, it->m_diffuse.y, it->m_diffuse.z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".specular").c_str()), it->m_specular.x, it->m_specular.y, it->m_specular.z);
            glUniform1f(glGetUniformLocation(m_program, (string + ".a").c_str()), it->m_a);
            glUniform1f(glGetUniformLocation(m_program, (string + ".b").c_str()), it->m_b);
            glUniform1f(glGetUniformLocation(m_program, (string + ".c").c_str()), it->m_c);
            i++;
        }
    }
    if(spotLights)
    {
        int i = 0;
        for(std::vector<SpotLight>::const_iterator it = spotLights->begin(); it != spotLights->end(); ++it)
        {
            std::string string = "spotLights[" + std::to_string(i) + "]";
            glUniform3f(glGetUniformLocation(m_program, (string + ".direction").c_str()), it->m_direction.x, it->m_direction.y, it->m_direction.z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".position").c_str()), it->m_transform->getPos_ref().x, it->m_transform->getPos_ref().y, it->m_transform->getPos_ref().z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".ambient").c_str()), it->m_ambient.x, it->m_ambient.y, it->m_ambient.z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".diffuse").c_str()), it->m_diffuse.x, it->m_diffuse.y, it->m_diffuse.z);
            glUniform3f(glGetUniformLocation(m_program, (string + ".specular").c_str()), it->m_specular.x, it->m_specular.y, it->m_specular.z);
            glUniform1f(glGetUniformLocation(m_program, (string + ".a").c_str()), it->m_a);
            glUniform1f(glGetUniformLocation(m_program, (string + ".b").c_str()), it->m_b);
            glUniform1f(glGetUniformLocation(m_program, (string + ".c").c_str()), it->m_c);
            glUniform1f(glGetUniformLocation(m_program, (string + ".innerAngle").c_str()), glm::cos(it->m_innerAngle));
            glUniform1f(glGetUniformLocation(m_program, (string + ".outerAngle").c_str()), glm::cos(it->m_outerAngle));
            i++;
        }
    }
}
