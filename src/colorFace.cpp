#include "colorFace.h"
#include "loadOBJ.h"
#include "ReadSim.h"
#include <math.h>
#include <stdlib.h> 
#include <sstream> 
#include <fstream> 
#include <QMenu>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMap>
#include <QCursor>


using namespace qglviewer;
using namespace std;

///////////////////////   V i e w e r  ///////////////////////
void Viewer::init()
{
  restoreStateFromFile();

  // Define 'Control+Q' as the new exit shortcut (default was 'Escape')
  setShortcut(EXIT_VIEWER, Qt::CTRL+Qt::Key_Q);

  // Set 'Control+F' as the FPS toggle state key.
  setShortcut(DISPLAY_FPS, Qt::CTRL+Qt::Key_F);

  // Add custom key description (see keyPressEvent).
  setKeyDescription(Qt::Key_U, "color contrast up");
  setKeyDescription(Qt::Key_D, "color contrast down");
  setKeyDescription(Qt::Key_W, "wireframe");
  setKeyDescription(Qt::Key_F, "flat shading");
  setKeyDescription(Qt::Key_C, "toggle between color channel");


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
        //std::cout << "(i,j) = " << i << " " << j << std::endl;
        //std::cout << "source1max is " << source1max << std::endl;
        //std::cout << "source2max is " << source2max << std::endl;
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
     double ratio2 = abs(data[i].source2[0])/source1max;

     particle_[i].col_r = ratio1; 
     particle_[i].col_b = ratio2;
  }

  glPointSize(5.0);
  setGridIsDrawn(false);
  setAnimationPeriod(15);
  startAnimation();
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
  // Get event modifiers key
  const Qt::KeyboardModifiers modifiers = e->modifiers();

  // A simple switch on e->key() is not sufficient if we want to take state key into account.
  // With a switch, it would have been impossible to separate 'F' from 'CTRL+F'.
  // That's why we use imbricated if...else and a "handled" boolean.
  bool handled = false;
  if ((e->key()==Qt::Key_W) && (modifiers==Qt::NoButton))
	{
	  wireframe_ = !wireframe_;
	  if (wireframe_)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	  else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	  handled = true;
	  updateGL();
	}
  else if ((e->key()==Qt::Key_F) && (modifiers==Qt::NoButton))
	  {
	flatShading_ = !flatShading_;
	if (flatShading_)
	  glShadeModel(GL_FLAT);
	else
	  glShadeModel(GL_SMOOTH);
	handled = true;
	updateGL();
	  }
  else if ((e->key()==Qt::Key_U) && (modifiers==Qt::NoButton))
	  {
          source1max = source1max*0.9; 
	  }
  else if ((e->key()==Qt::Key_D) && (modifiers==Qt::NoButton))
	  {
          source1max = source1max*1.1; 
	  }
  else if ((e->key()==Qt::Key_C) && (modifiers==Qt::NoButton))
	  {
          singleChannel_ = !singleChannel_;
          source1max = source1max*1.1; 
	  }

        
  // ... and so on with other else/if blocks.

  if (!handled)
	QGLViewer::keyPressEvent(e);
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
  std::cout << "time step: " << counter << std::endl;
  std::cout << "source1max = " << source1max << std::endl;
  for (int i=0; i<NCell; i++)
  {
    double ratio1 = abs(data[i].source1[counter])/source1max;
    double ratio2 = abs(data[i].source2[counter])/source1max;

    double ratio = (ratio1+ratio2)/2.0;

    if (singleChannel_)
    {
        particle_[i].col_r = ratio1; 
        particle_[i].col_g = 0.0f; 
        particle_[i].col_b = ratio2;
    }
    else
    {
        particle_[i].col_r = ratio; 
        particle_[i].col_g = ratio; 
        particle_[i].col_b = ratio;
    }
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
  glColor3f(col_r, col_g , col_b);
  glVertex3fv(pos_);
}


void Particle::init()
{
}
