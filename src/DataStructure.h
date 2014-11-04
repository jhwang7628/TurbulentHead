#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <iostream> 
#include <vector> 
#include "Vector3.h" 
#include "ReadSim.h"
#include <QGLViewer/qglviewer.h>

using namespace std; 




class Mesh; 
class surface; 
class tri; 
class vert; 
class Particle;


void assignK(Mesh *mesh);

class Mesh 
{
    public : 
        surface* meshSurface; 
        surface* extractedSurface;
        string mshName;
        void readMesh();
        void readOBJ(std::string filename);
        void ExtractSurface();
        void assignK();
        Mesh(string f) 
        {
            mshName = f;
        }
           
};

class surface  
{ 
    private : 
        string type; 
    public : 
        vector<tri> trilist; 
        vector<vert> vertlist;
        int Nnanvert;

        void findShareTriangles(const int ind_xi, const int ind_xj, const vector<tri>& triRange, vector<double>& alpha);
        void computeK(); 
        
        surface()
        { 
            type="mesh";
        }
            
        friend std::ostream &operator<<(std::ostream& os, const surface* s)
        {
            //os << "{" << V.x << ", " << V.y << ", " << V.z << "}"; 
            os << "Surface type: " << s->type; 
            return os;
        }

        void printOBJ(string objname);

        
};


class tri 
{
    public: 
        Vector3<int> index; // the index to the vertices
        Vector3<double> angle; // angle corresponding to each vertex of the triangle
        double area; 
        bool isObtuse; // one angle is greater than 90
        int ObtuseIndex;

        void printIndex() 
        {
            cout << "index of triangle = " << "(" << index.x << ", "  << index.y << ", " << index.z << ")" << endl;
        }

        bool equals(const tri& t) const
        {
            return (index.x == t.index.x && index.y == t.index.y && index.z == t.index.z);
        }

        void setEqual(tri t)
        { 
            angle = t.angle; 
            area  = t.area; 
            isObtuse = t.isObtuse; 
            ObtuseIndex = t.ObtuseIndex; 
        }

        tri()
        {
           area = 0; 
           isObtuse = false; 
           ObtuseIndex = 0;
        };

};

class neighbor
{
    public: 
        tri neighborTri; 
        double A_Voronoi; // corresponding to the neighborTri;
};


class vert  
{
    public: 
        Vector3<double> position; 
        Vector3<double> normal;
        vector<neighbor> N1neighbors; // stores information of the 1-ring neighbors
        double A_mixed; // see Mark Meyer's discrete curvature paper
        double curvature;
        bool isOnSurface; 
        bool isnan;
        Vector3<double> K_xi_wo_Amixed;
        int correspondVert; 

        void setEqual(vert v)
        { 
            position = v.position; 
            normal   = v.normal; 
            N1neighbors = v.N1neighbors; 
            A_mixed = v.A_mixed; 
        }

        void printDetailInfo()
        {
            cout << " ============ vertex information ============= " << endl;

            if (isOnSurface) { cout << "This vertex is on the surface. " << endl; } 
            else { cout << "This vertex is not on the surface. " << endl; } 

            cout << "Position = " << position << endl;
            cout << "Normal = " << normal << endl; 
            cout << "Curvature = " << curvature << endl; 

            cout << "----- N1 neighbors information -----" << endl; 
            cout << "It has " << N1neighbors.size() << " N1 neighboring triangles" << endl; 
            for (unsigned jj = 0; jj<N1neighbors.size(); jj++)
            {
                if (N1neighbors[jj].neighborTri.isObtuse)
                    cout << "Triangle # " << jj << " is obtuse" << endl;
                else
                    cout << "Triangle # " << jj << " is not obtuse" << endl;

                cout << "Triangle # " << jj << " area = "  << N1neighbors[jj].neighborTri.area << endl;
                cout << "Triangle # " << jj << " index = " << N1neighbors[jj].neighborTri.index << endl;
                cout << "Triangle # " << jj << " angle = " << N1neighbors[jj].neighborTri.angle << endl;
            }
        }

            

};

#endif

