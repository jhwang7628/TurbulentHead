#include "colorFace.h"
#include "loadOBJ.h"
#include "ReadSim.h"
#include <math.h>
#include <stdlib.h> 
#include <sstream> 
#include <fstream> 

using namespace qglviewer;
using namespace std;

///////////////////////   V i e w e r  ///////////////////////
void Viewer::init()
{
  restoreStateFromFile();
  glDisable(GL_LIGHTING);

  readData(); 
  Nts = data[0].pressure.size(); 
  NCell = data.size(); 
  std::cout << "Nts is " << Nts << std::endl;
  std::cout << "NCell is " << NCell << std::endl;
  counter=0;

  source1max=0; 
  source2max=0; 
  for (int i=0; i<NCell; i++) 
  {
     for (int j=0; j<Nts; j++) 
     {
        //std::cout << "scanning through... " << i << " " << j << std::endl;
        double s1 = data[i].source1[j];
        double s2 = data[i].source2[j];
        if (abs(s1)>source1max) { source1max=abs(s1); } 
        if (abs(s2)>source2max) { source2max=abs(s2); } 
        std::cout << "(i,j) = " << i << " " << j << std::endl;
        std::cout << "source1max is " << source1max << std::endl;
        std::cout << "source2max is " << source2max << std::endl;
     }
  }

  std::cout << "max source1 " << source1max << std::endl;
  std::cout << "max source2 " << source2max << std::endl;


  //data[0].printPressure();
  //data[1].printPressure();


  //std::cout << NCell << " " << Nts << std::endl;
    
    
  particle_ = new Particle[NCell];
  for (int i=0; i<NCell; i++) 
  { 
     particle_[i].pos_ = Vec(data[i].positions.x,
                              data[i].positions.y,
                              data[i].positions.z);
     
     double ratio1 = abs(data[i].source1[0])/source1max;
     double ratio2 = abs(data[i].source2[0])/source2max;

     particle_[i].col_r = ratio1; 
     particle_[i].col_b = ratio2;
  }

  glPointSize(5.0);
  setGridIsDrawn(false);
  setAnimationPeriod(100);
  startAnimation();
}

void Viewer::readData() 
{ 
   ReadSim(data); 
} 

void Viewer::draw()
{
  glBegin(GL_POINTS);
  for (int i=0; i<NCell; i++)
     particle_[i].draw();
  glEnd();
}

void Viewer::animate()
{
  counter ++; 
  if (counter >= Nts)
  {
     counter = 0;
  }
  std::cout << counter << std::endl;
  for (int i=0; i<NCell; i++)
  {
    double ratio1 = abs(data[i].source1[counter])/source1max;
    double ratio2 = abs(data[i].source2[counter])/source2max;

    particle_[i].col_r = ratio1; 
    particle_[i].col_b = ratio2;
    //particle_[i].animate(data[i].source1, data[i].source2);
  }
  std::cout << " color red is " <<  particle_[0].col_r << std::endl;
}

///////////////////////   P a r t i c l e   ///////////////////////////////

Particle::Particle()
{
  init();
}

void Particle::animate()
{
}

void Particle::draw()
{
  glColor3f(col_r, 0.0f , col_b);
  glVertex3fv(pos_);
}


void Particle::init()
{
}
