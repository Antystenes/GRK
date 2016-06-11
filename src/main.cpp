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

class Model
{
private:
    std::vector<Vertex> m_vertexData;
    GLuint m_vbo, m_vao;
public:
    Model() = delete;
    Model(std::vector<Vertex>);
    ~Model() = default;
    void Draw();
};

Model::Model(std::vector<Vertex> v)
{
    m_vertexData = v;
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(Vertex), &m_vertexData[0], GL_STREAM_DRAW);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
}

void Model::Draw()
{

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexData.size());
}

class Drawable
{
private:
    Model* m_model;
		float m_transformation[16];
		GLuint m_transformationUniform;
    GLuint m_shader;
public:
		Drawable() = delete;
		Drawable(Model*, GLuint,float, float, float);
		~Drawable() = default;
		void Translate(float x, float y, float z);
		void Draw();
};

Drawable::Drawable(Model* model, GLuint shader,float x, float y, float z)
{
    m_shader = shader;
		m_model = model;
		m_transformation[0] = m_transformation[5] = m_transformation[10] = m_transformation[15] = 1;
		m_transformation[12]+= x;
		m_transformation[13]+= y;
		m_transformation[14]+= z;
	  m_transformationUniform = glGetUniformLocation(shader, "transformation");
}

void Drawable::Draw()
{
    glUseProgram(m_shader);
		glUniformMatrix4fv(m_transformationUniform, 1, GL_FALSE, m_transformation);
		m_model->Draw();
}

void Drawable::Translate(float x, float y, float z)
{
		m_transformation[12]+= x;
		m_transformation[13]+= y;
		m_transformation[14]+= z;
}

class Drawer //pun intended CD
{
private:
    static std::vector <Model*> model;
    static std::vector <Drawable*> drawable;
    static GLuint shaders;
public:
    static void Init();
    static void Draw();
    static void AddModel(std::vector<Vertex>);
    static void AddDrawable(int i, float x=0, float y=0, float z=0);
    static Drawable* GetDrawable(int i) { return drawable[i];}
    static Model* GetModel(int i) { return model[i];};
		static GLuint GetShaders() { return shaders;};
};


void Drawer::AddDrawable(int i, float x, float y, float z)
{
    drawable.push_back(new Drawable(model[i], shaders,x,y,z));
};

void Drawer::AddModel(std::vector<Vertex> v)
{
    model.push_back(new Model(v));
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
    for(auto &i : drawable)
    {
        i->Draw();
    }
    glutSwapBuffers();
}
GLuint Drawer::shaders = 0;
std::vector <Model*> Drawer::model;
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
    Drawer::AddModel(trojkat);
//    Drawer::AddModel(trojkat);
    Drawer::AddDrawable(0, 0, 0 ,0);
    Drawer::AddDrawable(0,-0.3, -0.3, 0.0);
    Drawer::AddDrawable(0, 0.3, 0.3, 0.0);
//    Drawer::GetDrawable(0)->Translate(0.0, 0.0, 0.0);
    Drawer::GetDrawable(1)->Translate(-0.3, -0.3, 0.0);
//    Drawer::GetDrawable(2)->Translate(0.0f, -0.3f, 0.0f);
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
      case 'w':
         break;
   }
}

int main(int argc, char** argv)
{
    System system(argc, argv);
    return 0;
}
