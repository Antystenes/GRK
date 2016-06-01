#include "myShader.hpp"
#include <GL/glut.h>

namespace Graphics
{
  class Window
  {
  private:
    int width, height;
    char* name;
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

  class Drawer //pun intended xd
  {
  private:
    static GLuint shaders;
  public:
    static void Init();
    static void Draw();
  };

  void Drawer::Init()
  {
    glEnable(GL_DEPTH_TEST);
    shaders = LoadShader("shaders/Vertex_Shader.glsl", "shaders/Fragment_Shader.glsl");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  void Drawer::Draw()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
    glUseProgram(shaders);
    glDrawArrays(GL_TRIANGLES,0,3);
    glutSwapBuffers();
  }
  GLuint Drawer::shaders = 0;
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
  glutDisplayFunc(Graphics::Drawer::Draw);
  glutMainLoop();
}


int main(int argc, char **argv)
{
  System system(argc, argv);
  return 0;
}
