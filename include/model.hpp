#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GL/glut.h>
#include "objloader.hpp"

class Model
{
private:
    std::vector<Vertex> m_vertexData;
    GLuint m_shader, m_vbo, m_vao, m_vaotex, texture, texUniform;
public:
    Model() = delete;
    Model(std::string, GLuint, const char*);
    Model(std::vector<float> model, GLuint shader, const char* filename);
    ~Model() = default;
    void Draw();
    static GLuint LoadImage(std::string);
};


#endif //MODEL_HPP
