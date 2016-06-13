#include <cmath>
#include "drawer.hpp"
#include "configuration.hpp"
#include "myShader.hpp"


GLuint Drawer::shaders, Drawer::cameraUniform, Drawer::sharkShaders, Drawer::lightUniform, Drawer::perspectiveUniform = 0;
std::vector <std::unique_ptr<Model> > Drawer::model;
std::vector <std::unique_ptr<Drawable> > Drawer::drawable;
float Drawer::camera[] = {0.0001, 0.0, 0.0, 0.0,
                          0.0, 0.0001, 0.0, 0.0,
                          0.0, 0.0, 0.0001, 0.0,
                          0.0, 0.0, 0.0, 1.0};
float Drawer::perspective[];
float Drawer::light[] = {1000, 10000, 0.0, 1.0};


void Drawer::AddDrawable(int i, float x, float y, float z)
{
    drawable.emplace_back(new Drawable(model[i].get(), shaders,x,y,z));
}

void Drawer::AddModel(std::string v, const char* filename)
{
    model.emplace_back(new Model(v, shaders, filename));
}

void Drawer::AddModel(std::vector<float> thsi, const char* filename)
{
    model.emplace_back(new Model(thsi, sharkShaders, filename));
}

void Drawer::Init()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);
    sharkShaders = LoadShader(Configuration::Get().GetElement("back_v_path").c_str(),
                              Configuration::Get().GetElement("back_f_path").c_str());
    shaders = LoadShader(Configuration::Get().GetElement("vertex_path").c_str(),
                         Configuration::Get().GetElement("fragment_path").c_str());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUseProgram(shaders);
    cameraUniform = glGetUniformLocation(shaders, "camera");
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, camera);
    perspectiveUniform = glGetUniformLocation(shaders,"perspective");
    float fzNear = 0.1f;
    float fzFar = 3000000.0f;
    perspective[0] = 1.0;
    perspective[5] = 1.0;
    perspective[10] = (fzFar + fzNear) / (fzNear-fzFar);
    perspective[14] = (2 * fzFar * fzNear) / (fzNear-fzFar);
    perspective[11] = -1.0f;
    glUniformMatrix4fv(perspectiveUniform, 1, GL_FALSE, perspective);
    lightUniform = glGetUniformLocation(shaders, "light");
    glUniform4fv(lightUniform, 1, light);

}

void Drawer::MoveCamera(float x, float y, float z)
{
    camera[12]-=x;
    camera[13]-=y;
    camera[14]-=z;
    glUseProgram(shaders);
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, camera);
}

void Drawer::MoveLight(float x, float y, float z)
{
    light[0]+=x;
    light[1]+=y;
    light[2]+=z;
    glUseProgram(shaders);
    glUniform4fv(lightUniform, 1, light);
}

void Drawer::Draw()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static float t = 0;
    t += 0.1;
    GetDrawable(2)->Translate(0,100* sin(t), 0);
    float camPos[] = {camera[12],
                      camera[13],
                      camera[14]};
    glUseProgram(shaders);
    int u = glGetUniformLocation(shaders, "camPos");
    glUniform3fv(u, 3, camPos);

    u = glGetUniformLocation(shaders, "t");
    glUniform1fv(u, 1, &t);
    glUseProgram(sharkShaders);
    u = glGetUniformLocation(sharkShaders, "camPos");
    glUniform3fv(u, 3, camPos);

    for(auto &i : drawable)
    {
        i->Draw();
    }
    glutSwapBuffers();
    glutPostRedisplay();
}
