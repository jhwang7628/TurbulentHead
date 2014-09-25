#include <fstream> 
#include <sstream> 
#include <string> 
#include <vector>
#include "curvature.h" 



using namespace std; 

static const double PI=3.14159265359;

int main() {

    Mesh mesh; 

    mesh.readMesh();




    return 0; 
}

// data parser //
void Mesh::readMesh () 
{ 
    meshSurface = new surface();

    const char* filename = "jhwang.msh";

    ifstream inhead(filename, ios::in); 
    string line; 

    while(getline(inhead,line)) 
    {
        if (line.substr(0,6) == "(10 (1")
        {
            break; 
        }
    }

    getline(inhead, line);  // get rid of the "( " line
    while(getline(inhead, line)) 
    {
        if (line.substr(0,1) == ")")
        {
            break; 
        } 

        vert newVert; 

        istringstream iss(line); 
        double xBuff, yBuff, zBuff; 

        iss >> xBuff >> yBuff >> zBuff; 

        newVert.position.x = xBuff; newVert.position.y = yBuff; newVert.position.z = zBuff;

        meshSurface->vertlist.push_back(newVert); 


    }

    cout << meshSurface << endl;
    cout << "mesh filename: " << filename << endl;
    cout << "number of vertices read in: " << meshSurface->vertlist.size() << endl;


    while(getline(inhead,line)) 
    {
        if (line.substr(0,6) == "(13 (d")
        {
            break; 
        }
    }

    getline(inhead, line);  // get rid of the "( " line
    while(getline(inhead, line)) 
    {
        if (line.substr(0,1) == ")")
        {
            break; 
        } 

        istringstream iss(line); 


        int polygonEdge;
        iss >> polygonEdge; 
        int Ntri =  polygonEdge - 2; 
        int intBuff[polygonEdge];
        for (int i=0; i<polygonEdge; i++) 
        {
            iss >> hex >> intBuff[i]; 
        }

        for (int i=0; i<Ntri; i++)
        {
            tri newTri;
            int i0 = intBuff[0], 
                i1 = intBuff[i+1], 
                i2 = intBuff[i+2]; 

            newTri.index.x = i0;
            newTri.index.y = i1; 
            newTri.index.z = i2;

            // record neighbors for the vertex
            meshSurface->vertlist[i0].N1neighbors.push_back(newTri); 
            meshSurface->vertlist[i1].N1neighbors.push_back(newTri); 
            meshSurface->vertlist[i2].N1neighbors.push_back(newTri); 

            // push back the triangle into the list
            meshSurface->trilist.push_back(newTri); 

            double triArea; 
            Vector3<double> a1, a2; 

            // compute the area of the triangle
            Vector3<double> v0 = (meshSurface->vertlist)[i0].position;
            Vector3<double> v1 = (meshSurface->vertlist)[i1].position;
            Vector3<double> v2 = (meshSurface->vertlist)[i2].position;
            
            double cos01 = 
                
                , cos02, cos12;

            v1.sub(v0); 
            v2.sub(v0); 

            Vector3<double> v1_copy=v1, v2_copy=v2; 

            // 

            if (acos(cos_bt_v1v2) > PI/2.0) 
            { 
                newTri.isObtuse = true; 
                newTri.ObtuseIndex = i0; 
            } 

            //cout << "is triArea2 has the same x value as v1? " << (triArea2.x == v1.x) << endl;
            //cout << "is triArea2 equal to v1? " << (&triArea2 == &v1) << endl;


            triArea = 0.5*v1.cross(v2).norm();

            












            cout << newTri.index.x << " " << newTri.index.y << " " << newTri.index.z << endl;
        }



    }

    cout << "number of triangles read in: " << meshSurface->trilist.size() << endl;
}


