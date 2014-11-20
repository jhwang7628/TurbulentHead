#include <QGLViewer/qglviewer.h>
#include "ReadSim.h" 
#include <vector>

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

class Viewer : public QGLViewer
{
protected :
  virtual void draw();
  virtual void init();
  virtual void keyPressEvent(QKeyEvent *e);
  virtual void animate();
  virtual void readData(); 

private :
  int Nts; 
  int NCell;
  int counter;
  Particle* particle_;
  std::vector<vertexData> data; 
  double source1max, source2max; 
  bool wireframe_, flatShading_, singleChannel_;

};


