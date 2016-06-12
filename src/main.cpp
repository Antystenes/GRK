#include "myShader.hpp"
#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <string.h>
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

    float u;
    float v;

    Vertex(float fx, float fy, float fz, float fu, float fv) : x(fx), y(fy), z(fz), u(fu), v(fv) {};
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
    static GLuint LoadImage(const char*);
};

GLuint Model::LoadImage(const char* filename)
{
    ILuint imageID;
    GLuint textureID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);
    ilLoadImage(filename);
    ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 ilGetInteger(IL_IMAGE_FORMAT),
                 ilGetInteger(IL_IMAGE_WIDTH),
                 ilGetInteger(IL_IMAGE_HEIGHT),
                 0,
                 ilGetInteger(IL_IMAGE_FORMAT),
                 GL_UNSIGNED_BYTE,
                 ilGetData());
    ilDeleteImages(1, &imageID);
    return textureID;
}

Model::Model(std::vector<Vertex> v, GLuint shader, const char* filename)
{
    m_shader=shader;
    m_vertexData = v;
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(Vertex), &m_vertexData[0], GL_STREAM_DRAW);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) (3*sizeof(float)));
    texture = LoadImage(filename);
    texUniform = glGetUniformLocation(m_shader, "text");
}

void Model::Draw()
{

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(texUniform, /*GL_TEXTURE*/0);
    glBindTexture(GL_TEXTURE_2D, texture);
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
    memset(m_transformation, 0, 16);
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
    static GLuint shaders, cameraUniform, perspectiveUniform;
    static float camera[16], perspective[16];
public:
    static void Init();
    static void Draw();
    static void AddModel(std::vector<Vertex>, const char*);
    static void AddDrawable(int i, float x=0, float y=0, float z=0);
    static void MoveCamera(float x, float y, float z);
    static Drawable* GetDrawable(int i) { return drawable[i];}
    static Model* GetModel(int i) { return model[i];};
    static GLuint GetShaders() { return shaders;};
};


void Drawer::AddDrawable(int i, float x, float y, float z)
{
    drawable.push_back(new Drawable(model[i], shaders,x,y,z));
};

void Drawer::AddModel(std::vector<Vertex> v, const char* filename)
{
    model.push_back(new Model(v, shaders, filename));
}

void Drawer::Init()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    shaders = LoadShader(Configuration::Get().GetElement("vertex_path").c_str(),
                         Configuration::Get().GetElement("fragment_path").c_str());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUseProgram(shaders);
    cameraUniform = glGetUniformLocation(shaders, "camera");
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, camera);
    perspectiveUniform = glGetUniformLocation(shaders,"perspective");
    float fzNear = 0.1f;
    float fzFar = 30.0f;
    perspective[0] = 1.0;
    perspective[5] = 1.0;
    perspective[10] = (fzFar + fzNear) / (fzNear-fzFar);
    perspective[14] = (2 * fzFar * fzNear) / (fzNear-fzFar);
    perspective[11] = -1.0f;
    glUniformMatrix4fv(perspectiveUniform, 1, GL_FALSE, perspective);
}

void Drawer::MoveCamera(float x, float y, float z)
{
    camera[12]-=x;
    camera[13]-=y;
    camera[14]-=z;
    glUseProgram(shaders);
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, camera);
}

void Drawer::Draw()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(auto &i : drawable)
    {
        i->Draw();
    }
    glutSwapBuffers();
    glutPostRedisplay();
}
GLuint Drawer::shaders, Drawer::cameraUniform, Drawer::perspectiveUniform = 0;
std::vector <Model*> Drawer::model;
std::vector <Drawable*> Drawer::drawable;
float Drawer::camera[] = {1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0};
float Drawer::perspective[] = {0.0, 0.0, 0.0, 0.0,
                               0.0, 0.0, 0.0, 0.0,
                               0.0, 0.0, 0.0, 0.0,
                               0.0, 0.0, 0.0, 0.0};


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
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
    std::vector<Vertex> trojkat = {Vertex( 0.0f, 0.0f, 0.0f, 0.5f, 0.0f),
                                   Vertex(-0.1f, 0.3f, 0.0f, 0.0f, 0.75f),
                                   Vertex( 0.0f, 0.3f, 0.0f, 0.5f, 0.75f),
                                   Vertex( 0.0f, 0.0f, 0.0f, 0.5f, 0.0f),
                                   Vertex( 0.0f, 0.3f, 0.0f, 0.5f, 0.75f),
                                   Vertex( 0.1f, 0.3f, 0.0f, 1.0f, 0.75f),
                                   Vertex(-0.1f, 0.3f, 0.0f, 0.0f, 0.75f),
                                   Vertex(-0.05f, 0.4f, 0.0f,0.25f, 1.0f),
                                   Vertex( 0.0f, 0.3f, 0.0f, 0.5f, 0.75f),
                                   Vertex( 0.1f, 0.3f, 0.0f, 1.0f, 0.75f),
                                   Vertex( 0.0f, 0.3f, 0.0f, 0.5f, 0.75f),
                                   Vertex( 0.05f, 0.4f, 0.0f, 0.75f, 1.0f)
    };
    Drawer::AddModel(trojkat, "media/DRAGON.BMP");
    std::vector<Vertex> tile = {Vertex( 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
                                Vertex( 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
                                Vertex( 0.5f, 0.0f, 0.5f, 1.0f, 0.0f),
                                Vertex( 0.5f, 0.0f, 0.5f, 1.0f, 0.0f),
                                Vertex( 0.0f, 0.0f, 0.5f, 0.0f, 0.0f),
                                Vertex( 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)};
    Drawer::AddModel(tile, "media/Ocean Floor.jpg");
    Drawer::AddDrawable(0, 0, 0 ,-2);
    for(int i = -10; i<10; i++)
        for(int j = -10; j<10; j++)
            Drawer::AddDrawable(1, 0.5*j, -1, 0.5*i);
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
       Drawer::MoveCamera(0.0, 0.1, 0.0);
       break;
   case 's':
       Drawer::MoveCamera(0.0,-0.1, 0.0);
       break;
   case 'a':
       Drawer::MoveCamera(-0.1, 0.0, 0.0);
       break;
   case 'd':
       Drawer::MoveCamera( 0.1, 0.0, 0.0);
       break;
   case 'z':
       Drawer::MoveCamera(0.0, 0.0, 0.1);
       break;
   case 'x':
       Drawer::MoveCamera(0.0, 0.0,-0.1);
       break;
   case 'i':
       Drawer::GetDrawable(0)->Translate(0.0, 0.02, 0.0);
       break;
   case 'k':
       Drawer::GetDrawable(0)->Translate(0.0,-0.02, 0.0);
       break;
   case 'j':
       Drawer::GetDrawable(0)->Translate(-0.02, 0.0, 0.0);
       break;
   case 'l':
       Drawer::GetDrawable(0)->Translate( 0.02, 0.0, 0.0);
       break;
   }
}

int main(int argc, char** argv)
{
    System system(argc, argv);
    return 0;
}
