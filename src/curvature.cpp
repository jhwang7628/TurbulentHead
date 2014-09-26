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
    (mesh.meshSurface)->computeK();

    cout << "coming from the sky!!!" << endl;
    cout << (mesh.meshSurface)->trilist.size() << endl;




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

    // reading in the vertex position of the entire sim domain
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

    // reading in the index list for each triangle on the surface HumanHead-2 (ID:13)
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

            // compute the area of the triangle
            Vector3<double> v0 = (meshSurface->vertlist)[i0].position;
            Vector3<double> v1 = (meshSurface->vertlist)[i1].position;
            Vector3<double> v2 = (meshSurface->vertlist)[i2].position;
            
            Vector3<double> v01 = v1 - v0; 
            Vector3<double> v02 = v2 - v0; 
            Vector3<double> v12 = v2 - v1;

            //cout << "v1 = " << v1 << endl;
            //cout << "v01 = " << v01 << endl;


            double cos_t0 =  v01.dot(v02)/v01.norm()/v02.norm();
            double cos_t1 = -v01.dot(v12)/v01.norm()/v12.norm();
            double cos_t2 =  v02.dot(v12)/v02.norm()/v12.norm();

            newTri.angle.x = cos_t0; 
            newTri.angle.y = cos_t1; 
            newTri.angle.z = cos_t2;

            int count =0 ;
            if ( cos_t0<0 )
            {
               newTri.isObtuse = true;
               newTri.ObtuseIndex = i0;
               count ++; 
            }
            else if ( cos_t1<0 ) 
            {
               newTri.isObtuse = true;
               newTri.ObtuseIndex = i1;
               count ++; 
            }
            else if ( cos_t2<0 ) 
            {
               newTri.isObtuse = true;
               newTri.ObtuseIndex = i2;
               count ++; 
            }

            if (count >1)
            {
               cerr << "something is wrong. there is two obtuse angle for a triangle. " << endl;
               exit(1); 
            }

            // NOTE: in-place cross product applied here!
            Vector3<double> triNormal = v01.cross(v02);

            // add the area-weighted triangle normal to the vertex 
            meshSurface->vertlist[i0].normal.add(triNormal); 
            meshSurface->vertlist[i1].normal.add(triNormal); 
            meshSurface->vertlist[i2].normal.add(triNormal); 

            newTri.area = 0.5*triNormal.norm();


            cout << newTri.index.x << " " << newTri.index.y << " " << newTri.index.z << endl;
        }



    }

    cout << "number of triangles read in: " << meshSurface->trilist.size() << endl;
}

void findShareTriangles(const int ind_xi, const int ind_xj, const vector<tri>& triRange, vector<tri>& SharedTri, vector<double>& alpha)
{
   int N = triRange.size(); 
   for (int i=0; i<N; i++)
   {
      int count_match_ind = 0; 
      if (triRange[i].index.x == ind_xi) {count_match_ind++;}
      if (triRange[i].index.x == ind_xj) {count_match_ind++;}
      if (triRange[i].index.y == ind_xi) {count_match_ind++;}
      if (triRange[i].index.y == ind_xj) {count_match_ind++;}
      if (triRange[i].index.z == ind_xi) {count_match_ind++;}
      if (triRange[i].index.z == ind_xj) {count_match_ind++;}

      // found the triangle!
      if (count_match_ind == 2)
      {
         SharedTri.push_back(triRange[i]);

         int sum_index=triRange[i].index.x + triRange[i].index.y + triRange[i].index.z;
         // find alpha and beta

         //alpha.push_back(triRange[i].angle.)
      }

   }

}



void surface::computeK()
{

   int Ntri = (this->trilist).size();
   int Nvert = (this->vertlist).size();

   // mark the N1 neighbors for each vertex
   for (int i=0; i<Ntri; i++)
   {

      cout << "filling N1 neighbors for triangle " << i+1 << "/" << Ntri << endl;
      int i0 = trilist[i].index.x; 
      int i1 = trilist[i].index.y; 
      int i2 = trilist[i].index.z; 
      vertlist[i0].N1neighbors.push_back(trilist[i]);
      vertlist[i1].N1neighbors.push_back(trilist[i]);
      vertlist[i2].N1neighbors.push_back(trilist[i]);
   }

   // compute A_voronoi for each vertex
   for (int i=0; i<Nvert; i++)
   {
      // how many triangles does this vertex possess
      int N_N1neighbors = vertlist[i].N1neighbors.size();

      vertlist[i].A_Voronoi = 0;

      // list of all neighboring vertices
      vector<int> neighborVert;

      for (int j=0; j<N_N1neighbors; j++)
      {
         neighborVert.push_back(vertlist[i].N1neighbors[j].index.x);
         neighborVert.push_back(vertlist[i].N1neighbors[j].index.y);
         neighborVert.push_back(vertlist[i].N1neighbors[j].index.z);
      }
      if (N_N1neighbors != 0)
      {
         //cout << "neighborVert contains: "; 
         //for (vector<int>::iterator it=neighborVert.begin(); it!=neighborVert.end();++it)
         //   cout << " " << *it; 
         //cout << endl;

         sort(neighborVert.begin(), neighborVert.end()); 
         //cout << "sorted neighborVert contains: "; 
         //for (vector<int>::iterator it=neighborVert.begin(); it!=neighborVert.end();++it)
         //   cout << " " << *it; 
         //cout << endl;

         unique(neighborVert.begin(), neighborVert.end()); 
         //cout << "unique sorted neighborVert contains: "; 
         //for (vector<int>::iterator it=neighborVert.begin(); it!=neighborVert.end();++it)
         //   cout << " " << *it; 
         //cout << endl;
      }

      // scan through each neighboring vertex to compute voronoi area
      for (int j=0; j<neighborVert.size(); j++) 
      {
         int ind_xi = i, ind_xj = neighborVert[j]; 

         vector<tri> SharedTri;
         findShareTriangles(ind_xi, ind_xj, vertlist[i].N1neighbors, SharedTri);


         //vertlist[i].A_Voronoi += (cot(alpha)+cot(beta))*
      }





   }


   // Compute A_mixed area and curvature
   //for (int i=0; i<Nvert; i++) 
   //{

   //   vertlist[i].A_mixed = 0;

   //   int N_N1neighbors = vertlist[i].N1neighbors.size();
   //   
   //   for (int j=0; j<N_N1neighbors; j++) 
   //   {
   //      if (!vertlist[i].N1neighbors[j].isObtuse) 
   //      {
   //         vertlist[i].A_mixed += A_Voronoi; 
   //      }
   //      else 
   //      {
   //         if (vertlist[i].N1neighbors[j].ObtuseIndex == i)
   //         {
   //            vertlist[i].A_mixed += trilist[j].area / 2; 
   //         }
   //         else 
   //         {
   //            vertlist[i].A_mixed += trilist[j].area / 4; 
   //         }
   //      }
   //   }
   //}

















}


