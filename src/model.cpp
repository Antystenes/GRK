#include "model.hpp"
#include "objloader.hpp"
#include "myShader.hpp"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

GLuint Model::LoadImage(std::string filename)
{
    ILuint imageID;
    GLuint textureID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);
    ilLoadImage(filename.c_str());
    ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 ilGetInteger(IL_IMAGE_FORMAT),
                 ilGetInteger(IL_IMAGE_WIDTH),
                 ilGetInteger(IL_IMAGE_HEIGHT),
                 0,
                 ilGetInteger(IL_IMAGE_FORMAT),
                 GL_UNSIGNED_BYTE,
                 ilGetData());
    ilDeleteImages(1, &imageID);
    return textureID;
}

Model::Model(std::string model, GLuint shader, const char* filename)
{
    m_shader=shader;
    OBJLoader::LoadOBJFromFile(model, m_vertexData, m_uvData, m_normalData);
    glGenBuffers(1, &m_vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(glm::vec3), &m_vertexData[0], GL_STREAM_DRAW);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &m_vboUv);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUv);
    glBufferData(GL_ARRAY_BUFFER, m_uvData.size() * sizeof(glm::vec3), &m_uvData[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &m_vboNormal);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormal);
    glBufferData(GL_ARRAY_BUFFER, m_normalData.size() * sizeof(glm::vec3), &m_normalData[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    texture = LoadImage(filename);
    texUniform = glGetUniformLocation(m_shader, "text");
}

void Model::Draw()
{

    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(texUniform, /*GL_TEXTURE*/0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexData.size());
}
