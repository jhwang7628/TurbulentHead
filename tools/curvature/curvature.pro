TARGET = curvature 
CONFIG *= qt opengl release
QT *= opengl xml

HEADERS = curvature.h 
SOURCES = curvature.cpp


INCLUDEPATH *= /usr/include /home/jui-hsienwang/glm ../Math
LIBS *= -L/usr/lib -lQGLViewer
