#include "drawable.hpp"
#include <glm/gtc/type_ptr.hpp>

Drawable::Drawable(Model* model, GLuint shader,float x, float y, float z)
    : m_shader(shader), m_model(model)
{
    m_transformation = glm::mat4(1.0);
    m_transformation[3] = glm::vec4(x, y, z, 1);
    m_transformationUniform = glGetUniformLocation(shader, "transformation");
}

void Drawable::Draw()
{
    glUniformMatrix4fv(m_transformationUniform, 1, GL_FALSE, glm::value_ptr(m_transformation));
    m_model->Draw();
}

void Drawable::Translate(float x, float y, float z)
{
    m_transformation[3]+= glm::vec4(x, y, z, 0);
}
