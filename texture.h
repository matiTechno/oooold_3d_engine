#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture
{

public:

    Texture(const std::string& fileName);

    ~Texture();

    void bind(GLuint unit = 0);

private:
    GLuint m_texture;

};

#endif // TEXTURE_H
