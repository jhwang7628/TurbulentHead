#ifndef QGLVIEWERSTRUCTURE_H
#define QGLVIEWERSTRUCTURE_H

#include <iostream> 
#include <vector> 
#include "DataStructure.h"
#include "Vector3.h" 
#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer 
{
   private : 
      //surface* Head = new surface; 
      Mesh *mesh;
      int Nts; 
      int NCell;
      int counter;
      Particle* particle_;
      std::vector<vertexData> data; 
      double source1max, source2max; 
      bool wireframe_, flatShading_, singleChannel_;

   protected : 
      virtual void draw(); 
      virtual void init(); 
      virtual void keyPressEvent(QKeyEvent *e);
      void assignK(Mesh *mesh);
      double max_curv;
   public:
      Viewer()
      {
          mesh = new Mesh;
      }

};

class Particle
{
    public :
      Particle();
    
      void init();
      void draw();
      void animate();
      double col_r, col_b, col_g;
      qglviewer::Vec pos_;

};

#endif
