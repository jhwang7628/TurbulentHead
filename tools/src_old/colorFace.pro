TARGET = colorFace
CONFIG *= qt opengl release
QT *= opengl xml

HEADERS = colorFace.h loadOBJ.h ReadSim.h ReadSim2.h # WriteSim.h
SOURCES = main.cpp colorFace.cpp loadOBJ.cpp ReadSim.cpp ReadSim2.cpp # WriteSim.cpp

# HEADERS = colorFace.h loadOBJ.h ReadSim.h
# SOURCES = main.cpp colorFace.cpp loadOBJ.cpp ReadSim.cpp

INCLUDEPATH *= /usr/include /home/jui-hsien/glm/glm ../tools/Math
LIBS *= -L/usr/lib -lQGLViewer
