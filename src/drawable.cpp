#include "drawable.hpp"

Drawable::Drawable(Model* model, GLuint shader,float x, float y, float z)
    : m_shader(shader), m_model(model)
{
    memset(m_transformation, 0, 16);
    m_transformation[0] = m_transformation[5] = m_transformation[10] = m_transformation[15] = 1;
    m_transformation[12]+= x;
    m_transformation[13]+= y;
    m_transformation[14]+= z;
    m_transformationUniform = glGetUniformLocation(shader, "transformation");
}

void Drawable::Draw()
{
    glUniformMatrix4fv(m_transformationUniform, 1, GL_FALSE, m_transformation);
    m_model->Draw();
}

void Drawable::Translate(float x, float y, float z)
{
    m_transformation[12]+= x;
    m_transformation[13]+= y;
    m_transformation[14]+= z;
}
