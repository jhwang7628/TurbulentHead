/* Author: Jui-Hsien Wang 
 *         jw969@cornell.edu
 *
 * Date:   Nov 3rd, 2014
 *
 *
 */


#include <fstream> 
#include <sstream> 
#include <string> 
#include <vector>
#include <algorithm>
#include <stdio.h>
#include "DataStructure.h"

using namespace std; 

static const double PI=3.14159265359;

int main(int argc, char** argv)
{

    /* Mesh reader */ 
    
    string tmp("jhwang.msh");
    Mesh mesh(tmp);
    int Nts; 
    int NCell;
    int counter;
    vector<vertexData> data; 
    double source1max, source2max; 
    
    cout << " testing " << endl;
    mesh.readMesh();
    mesh.ExtractSurface();
    mesh.extractedSurface->printOBJ(tmp+".obj");
    
    /* Curvature calculation */ 
    mesh.assignK();
    double max_curv;
    
    /* Acoustic sources calculation */ 
    
    /* OpenGL rendering */
    
    
    return 0;
}
