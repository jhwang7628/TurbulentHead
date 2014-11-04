TARGET = ComputeSoundSource 
CONFIG *= qt opengl release
QT *= opengl xml

HEADERS = ComputeSoundSource.h DataStructure.h
SOURCES = ComputeSoundSource.cpp DataStructure.cpp


INCLUDEPATH *= /usr/include /home/jui-hsienwang/glm ../tools/Math ../tools/computeSoundSources
LIBS *= -L/usr/lib -lQGLViewer
