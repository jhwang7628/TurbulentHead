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
        // void readOBJ(const char* filename); 
        void readOBJ(std::string filename);
           
};

class surface  
{ 
    private : 
        string type; 
    public : 
        vector<tri> trilist; 
        vector<vert> vertlist;
        int Nnanvert;

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


class Viewer : public QGLViewer 
{
   protected : 
      virtual void draw(); 
      virtual void init(); 
      void assignK(Mesh *mesh);
      double max_curv;
      // virtual void animate();
   private : 
      //surface* Head = new surface; 
      Mesh *mesh;
   public:
      Viewer()
      {
          mesh = new Mesh;
      }

};



#endif
