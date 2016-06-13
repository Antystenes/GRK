#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <vector>
#include <memory>

#include "drawable.hpp" //Drawable, Model

class Drawer //pun indented XD
{
private:
    static std::vector <std::unique_ptr<Model> > model;
    static std::vector <std::unique_ptr<Drawable> > drawable;
    static GLuint shaders, cameraUniform, perspectiveUniform, lightUniform, sharkShaders;
    static float camera[16], perspective[16], light[4];
public:
    static void Init();
    static void Draw();
    static void AddModel(std::string v, const char* filename);
    static void AddDrawable(int i, float x=0, float y=0, float z=0);
    static void AddModel(std::vector<float> thsi, const char* filename);
    static void MoveCamera(float x, float y, float z);
    static Drawable* GetDrawable(int i) { return drawable[i].get();}
    static Model* GetModel(int i) { return model[i].get();}
    static GLuint GetShaders() { return shaders;}
    static void MoveLight(float x, float y, float z);
};
#endif //DRAWER_HPP
