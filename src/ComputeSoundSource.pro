TARGET = ComputeSoundSource 
CONFIG *= qt opengl debug
QT *= opengl xml

HEADERS = ComputeSoundSource.h DataStructure.h
SOURCES = ComputeSoundSource.cpp MeshReader.cpp ComputeCurvature.cpp DataStructure.cpp ReadSimulation.cpp ComputeSources.cpp WriteData.cpp SumSources.cpp


INCLUDEPATH *= /usr/include /home/jui-hsienwang/glm ../tools/Math ../tools/computeSoundSources
LIBS *= -L/usr/lib -lQGLViewer
