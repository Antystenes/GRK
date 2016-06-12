#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "model.hpp"

#include <cstring> //memset

class Drawable
{
private:
    GLuint m_shader;
    Model* m_model;
    float m_transformation[16];
    GLuint m_transformationUniform;

public:
    Drawable() = delete;
    Drawable(Model*, GLuint,float, float, float);
    ~Drawable() = default;
    void Translate(float x, float y, float z);
    void Draw();
};
#endif //DRAWABLE_HPP
