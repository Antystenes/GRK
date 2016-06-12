#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GL/glut.h>

struct Vertex
{
    float x;
    float y;
    float z;

    float u;
    float v;

    Vertex(float fx, float fy, float fz, float fu, float fv) //TODO: Replace Vertex with builtins
        : x(fx), y(fy), z(fz), u(fu), v(fv) {};
    ~Vertex() = default;
};

class Model
{
private:
    std::vector<Vertex> m_vertexData;
    GLuint m_shader, m_vbo, m_vao, m_vaotex, texture, texUniform;
public:
    Model() = delete;
    Model(std::vector<Vertex>, GLuint, const char*);
    ~Model() = default;
    void Draw();
    static GLuint LoadImage(std::string);
};


#endif //MODEL_HPP
