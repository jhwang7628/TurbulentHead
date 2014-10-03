TARGET = colorFace
CONFIG *= qt opengl release
QT *= opengl xml

HEADERS = colorFace.h loadOBJ.h ReadSim.h
SOURCES = main.cpp colorFace.cpp loadOBJ.cpp ReadSim.cpp

INCLUDEPATH += /Users/jui-hsienwang/libQGLViewer/libQGLViewer-2.5.3 /Users/jui-hsienwang/glm
LIBS += -F/Users/jui-hsienwang/Library/Frameworks -framework QGLViewer
