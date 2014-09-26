#ifndef CURVATURE_H
#define CURVATURE_H

#include <iostream>
#include <vector> 
#include "Vector3.h"


using namespace std; 

class mesh; 
class surface; 
class tri; 
class vert; 



class Mesh 
{
    public : 
        surface* meshSurface; 
        void readMesh();
           
};

class surface  
{ 
    private : 
        string type; 
    public : 
        vector<tri> trilist; 
        vector<vert> vertlist;
        void computeK(); 
        //void findShareTriangles(const int ind_xi, const int ind_xj, tri& triA, tri& triB); 
        void findShareTriangles(const int ind_xi, const int ind_xj, const vector<tri>& triRange, vector<tri>& SharedTri);

        surface(){ type="mesh";}
            
        friend std::ostream &operator<<(std::ostream& os, const surface* s)
        {
            //os << "{" << V.x << ", " << V.y << ", " << V.z << "}"; 
            int len_vertlist = s->vertlist.size(); 
            int len_trilist  = s->trilist.size(); 
            os << "Surface type: " << s->type; 
            return os;
        }

        
};


class tri 
{
    public: 
        Vector3<int> index; // the index to the vertices
        Vector3<double> angle; // angle corresponding to each vertex of the triangle
        double area; 
        bool isObtuse; // one angle is greater than 90
        int ObtuseIndex;

        friend std::ostream &operator<<(std::ostream& os, const tri& t)
        {
            //os << "{" << V.x << ", " << V.y << ", " << V.z << "}"; 
            //os << "Surface type: " << s->type << endl; 
            return os;
        }

        tri()
        {
           area = 0; 
           isObtuse = false; 
           ObtuseIndex = 0;
        };

};

class vert  
{
    public: 
        Vector3<double> position; 
        Vector3<double> normal;
        vector<tri> N1neighbors; // the one-ring neighbor triangles to this vertex
        double A_Voronoi;
        double A_mixed; // see Mark Meyer's discrete curvature paper
        double curvature;

        //friend std::ostream &operator<<(std::ostream& os, const vert & v)
        //{
        //    //os << "{" << V.x << ", " << V.y << ", " << V.z << "}"; 
        //    os << v.position << endl; 
        //    return os;
        //}
        //
};



#endif
