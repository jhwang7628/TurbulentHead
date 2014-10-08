TARGET = verify 
CONFIG *= qt opengl release
QT *= opengl xml

HEADERS = verify.h 
SOURCES = verify.cpp


INCLUDEPATH *= /usr/include /home/jui-hsienwang/glm ../../Math
LIBS *= -L/usr/lib -lQGLViewer
