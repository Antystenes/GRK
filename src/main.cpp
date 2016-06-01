#include "myShader.hpp"
#include <vector>
#include <GL/glut.h>

namespace Graphics
{
  class Window
  {
  private:
    int width, height;
    const char* name;
  public:
    Window() : width(1280), height(720), name("Yolo") {};
    ~Window() = default;
    void Init();
  };

  void Window::Init()
  {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(width, height);
    glutCreateWindow(name);
  }

  class Drawable
  {
  private:
    const float* m_vertexData;
    GLuint m_vbo, m_vao;
  public:
    Drawable() = delete;
    Drawable(const float* v);
    ~Drawable() = default;
    void Draw();
  };

  Drawable::Drawable(const float* v)
  {
    m_vertexData = v;
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*4, m_vertexData, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  }

  void Drawable::Draw()
  {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  class Drawer //pun intended xd
  {
  private:
    static std::vector <Drawable*> drawable;
    static GLuint shaders;
  public:
    static void Init();
    static void Draw();
    static void AddDrawable(const float*);
  };

  void Drawer::AddDrawable(const float* v)
  {
    drawable.push_back(new Drawable(v));
  }

  void Drawer::Init()
  {
    glEnable(GL_DEPTH_TEST);
    shaders = LoadShader("shaders/Vertex_Shader.glsl", "shaders/Fragment_Shader.glsl");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  void Drawer::Draw()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glUseProgram(shaders);
    for(auto &i : drawable)
      {
        i->Draw();
      }
    glutSwapBuffers();
  }
  GLuint Drawer::shaders = 0;
  std::vector <Drawable*> Drawer::drawable;

}

class System
{
private:
  Graphics::Window m_window;
public:
  System(int, char **);
  ~System() = default;
};

System::System(int argc, char **argv)
{
  glutInit(&argc, argv);
  Graphics::Window m_window;
  m_window.Init();
  glewInit();
  Graphics::Drawer::Init();
  float trojkat[] = {-0.2f, 0.0f, -1.0f, 1.0f,
                     -0.2f, 0.2f, -1.0f, 1.0f,
                     0.2f,  0.2f, -1.0f, 1.0f};
  Graphics::Drawer::AddDrawable(trojkat);
  glutDisplayFunc(Graphics::Drawer::Draw);
  glutMainLoop();
}


int main(int argc, char **argv)
{
  System system(argc, argv);
  return 0;
}
