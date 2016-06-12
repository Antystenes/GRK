#include "system.hpp"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <vector>

#include "drawer.hpp"


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
    Drawer::AddModel("media/tubarao1.obj", "media/DRAGON.BMP");
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
