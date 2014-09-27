#ifndef CURVATURE_H
#define CURVATURE_H

#include <iostream>
#include <vector> 
#include "Vector3.h"
#include <QGLViewer/qglviewer.h>

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
        //void printsomething(int ind_xi, int ind_xj, vector<tri>& triRange, vector<double>& alpha);
        void findShareTriangles(const int ind_xi, const int ind_xj, const vector<tri>& triRange, vector<double>& alpha);
        void computeK(); 
        //void findShareTriangles(const int ind_xi, const int ind_xj, tri& triA, tri& triB); 

        void init(); 
        void draw(); 
        
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

        void printIndex() 
        {
            cout << "index of triangle = " << "(" << index.x << ", "  << index.y << ", " << index.z << ")" << endl;
        }

        bool equals(const tri& t) const
        {
            return (index.x == t.index.x && index.y == t.index.y && index.z == t.index.z);
        }

        //friend std::ostream &operator<<(std::ostream& os, const tri& t)
        //{
        //    //os << "{" << V.x << ", " << V.y << ", " << V.z << "}"; 
        //    //os << "Surface type: " << s->type << endl; 
        //    return os;
        //}

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
        bool isOnSurface; 

        //friend std::ostream &operator<<(std::ostream& os, const vert & v)
        //{
        //    //os << "{" << V.x << ", " << V.y << ", " << V.z << "}"; 
        //    os << v.position << endl; 
        //    return os;
        //}
        //
};


class Viewer : public QGLViewer 
{
   protected : 
      virtual void draw(); 
      virtual void init(); 
      // virtual void animate();
   private : 
      //surface* Head = new surface; 
      Mesh *mesh = new Mesh;


};



#endif
