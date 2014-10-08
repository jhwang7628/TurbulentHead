TARGET = keyboardAndMouse 
CONFIG *= qt opengl release
QT *= opengl xml

HEADERS = keyboardAndMouse.h
SOURCES = main.cpp keyboardAndMouse.cpp

INCLUDEPATH *= /usr/include /home/jui-hsien/glm/glm 
LIBS *= -L/usr/lib -lQGLViewer
