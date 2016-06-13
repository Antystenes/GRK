#include "system.hpp"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <vector>

#include "drawer.hpp"
#include "configuration.hpp"

System::System(int argc, char** argv)
{
    glutInit(&argc, argv);
    m_window.Init();
    glewInit();
    Drawer::Init();
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
    Drawer::AddModel(
        std::vector<float>{-1.0, -1.0 ,0.5,
                1.0, -1.0, 0.99,
                -1.0, 1.0, 0.99,
                1.0, -1.0, 0.99,
                1.0, 1.0, 0.99,
                -1.0, 1.0, 0.99},
        CONF_GET("g1"));
    Drawer::AddModel(CONF_GET("fish_obj"),
                     CONF_GET("fish_texture"));
    Drawer::AddDrawable(1, 1500, -2000, -6000);
    Drawer::AddDrawable(1, 1500, -2000, -6000);
    Drawer::AddDrawable(0, 0, 0, 0);
//    Drawer::MoveCamera(0, -2, 2);
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
       Drawer::GetDrawable(1)->Translate(0.0, 200, 0.0);
       break;
   case 'k':
       Drawer::GetDrawable(1)->Translate(0.0,-200, 0.0);
       break;
   case 'j':
       Drawer::GetDrawable(1)->Translate(-200, 0.0, 0.0);
       break;
   case 'l':
       Drawer::GetDrawable(1)->Translate( 200, 0.0, 0.0);
       break;
   case 'f':
       Drawer::MoveLight(-200, 0, 0);
       break;
   case 'h':
       Drawer::MoveLight(200, 0 , 0);
       break;
   case 't':
       Drawer::MoveLight(0, 2000, 0);
       break;
   case 'g':
       Drawer::MoveLight(0,-2000, 0);
       break;
   }
}
