#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

class Model
{
private:
    std::vector<glm::vec3> m_vertexData;
    std::vector<glm::vec2> m_uvData;
    std::vector<glm::vec3> m_normalData;
    GLuint m_shader, m_vboVertex, m_vboUv, m_vboNormal, m_vao, m_vaotex, texture, texUniform;
public:
    Model() = delete;
    Model(std::string, GLuint, const char*);
    ~Model() = default;
    void Draw();
    static GLuint LoadImage(std::string);
};


#endif //MODEL_HPP
