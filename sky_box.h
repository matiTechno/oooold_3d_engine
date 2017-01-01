#ifndef SKY_BOX_H
#define SKY_BOX_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include "stb_image.h"

class SkyBox
{
public:

    SkyBox(const std::vector<std::string>& fileNames)
    {
        // right, left, up, down, back, front

        glGenTextures(1, &m_skyBox);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyBox);

        for(GLuint i = 0; i < fileNames.size(); i++)
        {
            int width, height;

            unsigned char* imageData = stbi_load(fileNames[i].c_str(), &width, &height, 0, 4);

            if(imageData == 0)
                std::cout << "could not load texture " << fileNames[i] << std::endl;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            stbi_image_free(imageData);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    }
    ~SkyBox()
    {
        glDeleteTextures(1, &m_skyBox);
    }
    void bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyBox);
    }

private:

    GLuint m_skyBox;

};

#endif // SKY_BOX_H
