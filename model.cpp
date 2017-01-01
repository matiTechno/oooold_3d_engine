#include "model.h"
#include <iostream>
#include <stb_image.h>

Model::~Model()
{
    for(GLuint i = 0; i < m_meshes.size(); i++)
    {
        glDeleteVertexArrays(1, &(m_meshes[i].m_VAO));
        glDeleteBuffers(1, &(m_meshes[i].m_EBO));
        glDeleteBuffers(1, &(m_meshes[i].m_VBO));
    }
    for(GLuint i = 0; i < m_loaded_textures.size(); i++)
    {
        glDeleteTextures(1, &(m_loaded_textures[i].id));
    }
}

void Model::loadModel(const std::string& full_fileName)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(full_fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "could not load model " << full_fileName << std::endl;
        return;
    }
    m_directory = full_fileName.substr(0, full_fileName.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

ModTexMesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<ModVertex> vertices;
    std::vector<GLuint> indices;
    std::vector<ModTexture> textures;

    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        ModVertex vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;

        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;

        vertex.normal = vector;

        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vector1;
            vector1.x = mesh->mTextureCoords[0][i].x;
            vector1.y = mesh->mTextureCoords[0][i].y;

            vertex.texCoord = vector1;
        }
        else
            vertex.texCoord = glm::vec2();

        vertices.push_back(vertex); // vertices
    }

    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]); // indices
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<ModTexture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "mat_tex_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<ModTexture> specularMaps = loadTextures(material, aiTextureType_SPECULAR, "mat_tex_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end()); // textures

    return ModTexMesh(vertices, indices, textures);
}

std::vector<ModTexture> Model::loadTextures(aiMaterial* material, aiTextureType type, const std::string& typeName)
{
    std::vector<ModTexture> textures;
    for(GLuint i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        bool skip = false;
        for(GLuint j = 0; j < m_loaded_textures.size(); j++)
        {
            if(m_loaded_textures[j].fileName == str)
            {
                textures.push_back(m_loaded_textures[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            std::string fileName = str.C_Str();
            ModTexture texture;
            texture.id = stbiLoadTexture(fileName, m_directory);
            texture.type = typeName;
            texture.fileName = str;

            textures.push_back(texture);
            m_loaded_textures.push_back(texture);
        }
    }
    return textures;
}

GLuint Model::stbiLoadTexture(const std::string& fileName, const std::string& m_directory)
{
    GLuint texID;

    std::string loadPath = m_directory + '/' + fileName;

    int width, height;

    unsigned char* imageData = stbi_load(loadPath.c_str(), &width, &height, 0, 4);

    if(imageData == 0)
        std::cout << "could not load texture " << loadPath << std::endl;

    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(imageData);

    return texID;
}
