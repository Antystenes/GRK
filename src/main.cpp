#include "myShader.hpp"
#include <vector>
#include <GL/glut.h>
#include "configuration.hpp"

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
    Configuration::Get().Init();
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(width, height);
    glutCreateWindow(name);
}

struct Vertex
{
    float x;
    float y;
    float z;

    Vertex(float fx, float fy, float fz) : x(fx), y(fy), z(fz) {};
    ~Vertex() = default;
};

class Drawable
{
private:
    std::vector<Vertex> m_vertexData;
    GLuint m_vbo, m_vao;
public:
    Drawable() = delete;
    Drawable(std::vector<Vertex>);
    ~Drawable() = default;
    void Draw();
    void Translate(float, float, float);
};

Drawable::Drawable(std::vector<Vertex> v)
{
    m_vertexData = v;
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(Vertex), &m_vertexData[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
}

void Drawable::Draw()
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexData.size());
}

void Drawable::Translate(float x, float y, float z)
{

}

class Drawer //pun intended CD
{
private:
    static std::vector <Drawable*> drawable;
    static GLuint shaders;
public:
    static void Init();
    static void Draw();
    static void AddDrawable(std::vector<Vertex>);
};

void Drawer::AddDrawable(std::vector<Vertex> v)
{
    drawable.push_back(new Drawable(v));
}

void Drawer::Init()
{
    glEnable(GL_DEPTH_TEST);
    shaders = LoadShader(Configuration::Get().GetElement("vertex_path").c_str(),
                         Configuration::Get().GetElement("fragment_path").c_str());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Drawer::Draw()
{
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaders);
    for(auto &i : drawable)
    {
        i->Draw();
    }
    glutSwapBuffers();
}
GLuint Drawer::shaders = 0;
std::vector <Drawable*> Drawer::drawable;

class System
{
private:
   Window m_window;
   static void Keyboard(unsigned char, int, int);

public:
    System(int, char**);
    ~System() = default;
};

System::System(int argc, char** argv)
{
    glutInit(&argc, argv);
    Window m_window;
    m_window.Init();
    glewInit();
    Drawer::Init();
    std::vector<Vertex> trojkat = {Vertex(-0.2f, 0.0f, -1.0f),
                                   Vertex(-0.2f, 0.2f, -1.0f),
                                   Vertex(0.2f,  0.2f, -1.0f)};
    Drawer::AddDrawable(trojkat);
    glutDisplayFunc(Drawer::Draw);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
}

void System::Keyboard(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 27:
      case 'q':
         glutDestroyWindow(1);
      break;
   }
}

int main(int argc, char** argv)
{
    System system(argc, argv);
    return 0;
}
