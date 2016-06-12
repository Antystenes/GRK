#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "model.hpp"
#include <glm/glm.hpp>
#include <cstring> //memset

class Drawable
{
private:
    GLuint m_shader;
    Model* m_model;
    glm::mat4 m_transformation;
    GLuint m_transformationUniform;

public:
    Drawable() = delete;
    Drawable(Model*, GLuint,float, float, float);
    ~Drawable() = default;
    void Translate(float x, float y, float z);
    void Draw();
};
#endif //DRAWABLE_HPP
