#ifndef VERIFY_H
#define VERIFY_H

#include <iostream>
#include <vector> 
#include "Vector3.h"
#include <QGLViewer/qglviewer.h>

class vert
{
    public : 
        Vector3<double> coord; 
        double curvature; 
};


class Viewer : public QGLViewer 
{
   protected : 
      virtual void draw(); 
      virtual void init(); 
      std::vector<vert> vertlist;

      // virtual void animate();

};


#endif
