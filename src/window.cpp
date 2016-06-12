#include "window.hpp"
#include "configuration.hpp"

#include <GL/glut.h>

void Window::Init()
{
    Configuration::Get().Init();
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(width, height);
    glutCreateWindow(name.c_str());
}
