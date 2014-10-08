#include <fstream> 
#include <sstream> 
#include <string> 
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <qapplication.h>
#include "verify.h" 


using namespace qglviewer;
using namespace std; 

int main(int argc, char** argv)
{
   QApplication application(argc,argv);


   Viewer viewer;

   viewer.setWindowTitle("Curvature Contour on the head");

   viewer.show();


  return application.exec();
  //return 0;
}

void Viewer::init()
{
   restoreStateFromFile(); 
   glDisable(GL_LIGHTING);

   glPointSize(6.0); 
   setGridIsDrawn(false); 

   string filename;
   cout << "Curvature filename to-read: " << endl;
   getline(cin, filename);

   ifstream in(filename.c_str());

   if (!in) { cout << "cannot open file " << filename << endl; exit(1); }


   string line; 

   while(getline(in, line))
   {
       istringstream iss(line); 

       vert newVert; 

       iss >> newVert.coord.x >> newVert.coord.y >> newVert.coord.z ;
       iss >> newVert.curvature;

       vertlist.push_back(newVert);
   }
}


 void Viewer::draw()
 {
 
    //double max_curv = 0;
    //for (unsigned long i=0; i<mesh->meshSurface->vertlist.size(); i++)
    //{
    //   if (mesh->meshSurface->vertlist[i].isOnSurface)
    //   {
    //        if (abs(mesh->meshSurface->vertlist[i].curvature) > max_curv)
    //        {
    //           max_curv = abs(mesh->meshSurface->vertlist[i].curvature);
    //           cout << "max_curv is updated in vert" << i << " to be " <<  max_curv << endl;
    //        }
    //   }
    //}

    //double max_curv = mesh->meshSurface->vertlist[2151].curvature; 
    double max_curv = 200; 

    ////cout << max_curv << endl;


 
    glBegin(GL_POINTS);
 
    for (unsigned long i=0; i<vertlist.size(); i++) 
    {

          //cout << "i=" << i << endl;
          double ratio = (vertlist[i].curvature/max_curv);
          //cout << ratio << endl;
          glColor3f(ratio, 0.0, -ratio);
          qglviewer::Vec vertPos = Vec(
                vertlist[i].coord.x,
                vertlist[i].coord.y,
                vertlist[i].coord.z);
          glVertex3fv(vertPos); 
    }
    glEnd(); 
 
 
    //glBegin(GL_TRIANGLES);
    //for (unsigned long i=0; i<(mesh->meshSurface)->trilist.size(); i++) 
    //{
    //   //if ((mesh->meshSurface->vertlist)[i].isOnSurface)
    //  //{
    //      glColor3f(0.3f, 0.3f, 0.3f); 
    //      qglviewer::Vec vertPos = Vec(
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.x].position.x*0.99,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.x].position.y*0.99,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.x].position.z*0.99);
    //      glVertex3fv(vertPos); 
    //      qglviewer::Vec vertPos2 = Vec(
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.y].position.x*0.99,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.y].position.y*0.99,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.y].position.z*0.99);
    //      glVertex3fv(vertPos2); 
    //      qglviewer::Vec vertPos3 = Vec(
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.z].position.x*0.99,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.z].position.y*0.99,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.z].position.z*0.99);
    //      glVertex3fv(vertPos3); 
    //   //}
    //}
 
    //glEnd(); 
 
    //for (unsigned long i=0; i<(mesh->meshSurface)->trilist.size(); i++) 
    //{
    //   //if ((mesh->meshSurface->vertlist)[i].isOnSurface)
    //  //{
    //      glBegin(GL_LINES);
    //      glColor3f(0.0f, 0.0f, 0.0f); 
    //      qglviewer::Vec vertPos = Vec(
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.x].position.x,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.x].position.y,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.x].position.z);
    //      glVertex3fv(vertPos); 
    //      qglviewer::Vec vertPos2 = Vec(
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.y].position.x,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.y].position.y,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.y].position.z);
    //      glVertex3fv(vertPos2); 
    //      qglviewer::Vec vertPos3 = Vec(
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.z].position.x,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.z].position.y,
    //            mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.z].position.z);
    //      glVertex3fv(vertPos3); 
    //   //}
    //      glEnd(); 
    //}
 
    cout << "see drawing" << endl;
}




   


