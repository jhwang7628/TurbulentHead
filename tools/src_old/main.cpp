#include "colorFace.h"
#include <iostream> 
//#include "loadOBJ.h"
#include "ReadSim.h" 
#include <qapplication.h>
#include <vector>

int main(int argc, char** argv)
{
   QApplication application(argc,argv);

   Viewer viewer;

   viewer.setWindowTitle("Pressure Contour on the head");

   viewer.show();


  return application.exec();
  //return 0;
}

