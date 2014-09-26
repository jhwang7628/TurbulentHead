#include <fstream> 
#include <sstream> 
#include <string> 
#include <vector>
#include <algorithm>
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

        newVert.isOnSurface=false;

        istringstream iss(line); 
        double xBuff, yBuff, zBuff; 

        iss >> xBuff >> yBuff >> zBuff; 

        newVert.position.x = xBuff; newVert.position.y = yBuff; newVert.position.z = zBuff;

        meshSurface->vertlist.push_back(newVert); 

    }

    cout << meshSurface << endl;
    cout << "mesh filename: " << filename << endl;
    cout << "number of vertices read in: " << meshSurface->vertlist.size() << endl;

    //ofstream offah("havetofindthisbug.txt"); 

    while(getline(inhead,line)) 
    {
        if (line.substr(0,6) == "(13 (d")
        {
            break; 
        }
    }


    // reading in the index list for each triangle on the surface HumanHead-2 (ID:13)
    int stop_count =0, tri_count=0, push_back_count=0;
    getline(inhead, line);  // get rid of the "( " line
    while(getline(inhead, line)) 
    {
        if (line.substr(0,1) == ")")
        {
            break; 
        } 
        istringstream iss(line); 


        int polygonEdge;
        iss >> hex >> polygonEdge; 
        int Ntri =  polygonEdge - 2; 
        vector<int> intBuff(polygonEdge);
        for (int i=0; i<polygonEdge; i++) 
        {
            iss >> hex >> intBuff[i]; 
        }

        //offah << polygonEdge << endl;

        for (int i=0; i<Ntri; i++)
        {
            tri newTri;
            tri_count++;
            int i0 = intBuff[0], 
                i1 = intBuff[i+1], 
                i2 = intBuff[i+2]; 

            newTri.index.x = i0;
            newTri.index.y = i1; 
            newTri.index.z = i2;

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

            newTri.angle.x = acos(cos_t0); 
            newTri.angle.y = acos(cos_t1); 
            newTri.angle.z = acos(cos_t2);


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
               cerr << "something is wrong. there are two obtuse angles for a single triangle. " << endl;
               exit(1); 
            }

            // NOTE: in-place cross product applied here!
            Vector3<double> triNormal = v01.cross(v02);

            // add the area-weighted triangle normal to the vertex 
            meshSurface->vertlist[i0].normal.add(triNormal); 
            meshSurface->vertlist[i1].normal.add(triNormal); 
            meshSurface->vertlist[i2].normal.add(triNormal); 

            //cout << "isOnSurface of this vertex "  << meshSurface->vertlist[i0].isOnSurface << endl;
            meshSurface->vertlist[i0].isOnSurface = true; 
            meshSurface->vertlist[i1].isOnSurface = true; 
            meshSurface->vertlist[i2].isOnSurface = true; 

            newTri.area = 0.5*triNormal.norm();

            // push back the triangle into the list
            //cout << "newTri.angle = " << newTri.angle << endl;

            meshSurface->trilist.push_back(newTri); 
            meshSurface->vertlist[i0].N1neighbors.push_back(newTri); 
            meshSurface->vertlist[i1].N1neighbors.push_back(newTri); 
            meshSurface->vertlist[i2].N1neighbors.push_back(newTri); 

            push_back_count += 3;





            //newTri.printIndex();
        }


    }

    //offah.close();
    cout << "number of triangles read in: " << meshSurface->trilist.size() << endl;
}

// return alpha for this pair of indices ind_xi and ind_xj
void surface::findShareTriangles(const int ind_xi, const int ind_xj, const vector<tri>& triRange, vector<double>& alpha)
{
   //cout << "-------------------- for (" <<  ind_xi << ", " << ind_xj << ") ----------------------" << endl;

   int N = triRange.size(); 

   for (int i=0; i<N; i++)
   {
       //cout << "triangle " << &triRange[i];
       // use concept from chmod: 1 2 4 represents x, y, z
       int count_match_ind = 0, index_match = 0;
       if (triRange[i].index.x == ind_xi) {count_match_ind++;index_match += 1;}
       if (triRange[i].index.x == ind_xj) {count_match_ind++;index_match += 1;}
       if (triRange[i].index.y == ind_xi) {count_match_ind++;index_match += 2;}
       if (triRange[i].index.y == ind_xj) {count_match_ind++;index_match += 2;}
       if (triRange[i].index.z == ind_xi) {count_match_ind++;index_match += 4;}
       if (triRange[i].index.z == ind_xj) {count_match_ind++;index_match += 4;}

       // found the triangle!
       if (count_match_ind == 2)
       {
          //cout << " is the matched triangle. " << endl;
          cout << "found the triangle" << endl; 
          double Tri_alpha;

          //cout << "the angle for this triangle is " << triRange[i].angle << endl;

          if (index_match == 3) // the other index is z
          {
              //cout << "alpha at z " << endl; 
              Tri_alpha = triRange[i].angle.z; 
          }
          else if (index_match == 6) // the other index is x
          {
              //cout << "alpha at x " << endl; 
              Tri_alpha = triRange[i].angle.x; 
          }
          else if (index_match == 5) // the other index is y
          {
              //cout << "alpha at y " << endl; 
              Tri_alpha = triRange[i].angle.y; 
          }
          else 
          {
              cout << "ind_xi = " << ind_xi << "; ind_xj = " << ind_xj << endl;
              cout << index_match << endl;
              cerr << "something is wrong. cannot find alpha for this triangle " << endl;
              exit(1);
          }

          alpha.push_back(Tri_alpha);

          //cout << " The triangle that contains this edge (" << ind_xi << " and " << ind_xj << " is " << &triRange[i] << endl;
       }
       //else
       //{
       //    cout << " is NOT the matched triangle. " << endl;
       //}

   }

   //cout << "size of alpha is " << alpha.size() << endl;

}



void surface::computeK()
{

   int Ntri = (this->trilist).size();
   int Nvert = (this->vertlist).size();

   // mark the N1 neighbors for each vertex
   //for (int i=0; i<Ntri; i++)
   //{

   //   cout << "filling N1 neighbors for triangle " << i+1 << "/" << Ntri << endl;
   //   int i0 = trilist[i].index.x; 
   //   int i1 = trilist[i].index.y; 
   //   int i2 = trilist[i].index.z; 
   //   vertlist[i0].N1neighbors.push_back(trilist[i]);
   //   vertlist[i1].N1neighbors.push_back(trilist[i]);
   //   vertlist[i2].N1neighbors.push_back(trilist[i]);
   //}

   // compute A_voronoi for each vertex
   ofstream outfile("A_Voronoi.txt");
   for (int i=0; i<Nvert; i++)
   {
      if (vertlist[i].isOnSurface) 
      {
          //cout << "not on surface " << endl;
      

          // how many triangles does this vertex possess
          int N_N1neighbors = vertlist[i].N1neighbors.size();

          vertlist[i].A_Voronoi = 0;

          // list of all neighboring vertices
          vector<int> neighborVert;

          for (int j=0; j<N_N1neighbors; j++)
          {
             //cout << "angles of the N1-neighbors are " << vertlist[i].N1neighbors[j].angle << endl;
             neighborVert.push_back(vertlist[i].N1neighbors[j].index.x);
             neighborVert.push_back(vertlist[i].N1neighbors[j].index.y);
             neighborVert.push_back(vertlist[i].N1neighbors[j].index.z);
          }
          if (N_N1neighbors != 0)
          {
             sort(neighborVert.begin(), neighborVert.end()); 
             unique(neighborVert.begin(), neighborVert.end()); 
          }

          // scan through each neighboring vertex to compute voronoi area
          double sum_N1AV = 0;
          for (int j=0; j<neighborVert.size() && (neighborVert[j]!=i); j++) 
          {
             int ind_xi = i, ind_xj = neighborVert[j]; 

             // find alpha and beta for each neighbor
             vector<double> Alpha; 

             findShareTriangles(ind_xi, ind_xj, vertlist[i].N1neighbors, Alpha);
             cout << "size of Alpha is " << Alpha.size() << endl;
             cout << "Retrieved Alpha is " << Alpha[0] << " and " << Alpha[1] << endl;
             //if (Alpha[0] < 0 || Alpha[0] > PI || Alpha[1] < 0 || Alpha[1] > PI) 
             //{
             //    cerr << "something is wrong. alpha is greater than 180 or negative. " << endl; 
             //    exit(1); 
             //}

             double xij2 = (vertlist[ind_xi].position - vertlist[ind_xj].position).normSq();
             //if (xij2<0) 
             //{
             //    cerr << "something is wrong. xij2 is negative. " << endl; 
             //    exit(1); 
             //}
             //cout << "alpha = " << Alpha[0] << ", beta = " << Alpha[1] << endl;
             double tmp = (1.0/tan(Alpha[0]) + 1.0/tan(Alpha[1]))*xij2;
             if (tmp>0) 
             {
                 sum_N1AV += tmp;
                 //cerr << "something is wrong. tmp is negative. " << endl; 
                 //exit(1); 
             }
          }
          vertlist[i].A_Voronoi = 1.0/8.0*sum_N1AV;
          //if (vertlist[i].A_Voronoi<0) 
          //{
          //    cerr << "something is wrong. cell Voronoi area is negative. " << endl; 
          //    //exit(1); 
          //}
          cout << "the Voronoi area for vertex " << i << " is " << vertlist[i].A_Voronoi << endl;
          outfile << vertlist[i].A_Voronoi << endl;


      }

   }
   outfile.close();


   // Compute A_mixed area and curvature
   for (int i=0; i<Nvert; i++) 
   {
      if (vertlist[i].isOnSurface)
      {

          vertlist[i].A_mixed = 0;

          int N_N1neighbors = vertlist[i].N1neighbors.size();
          
          for (int j=0; j<N_N1neighbors; j++) 
          {
             if (!vertlist[i].N1neighbors[j].isObtuse) 
             {
                vertlist[i].A_mixed += vertlist[i].A_Voronoi; 
             }
             else 
             {
                if (vertlist[i].N1neighbors[j].ObtuseIndex == i)
                {
                   cout << "trilist[j].area = " << trilist[j].area << endl;
                   vertlist[i].A_mixed += trilist[j].area / 2.0; 
                }
                else 
                {
                   cout << "trilist[j].area = " << trilist[j].area << endl;
                   vertlist[i].A_mixed += trilist[j].area / 4.0; 
                }
             }
          }

      //vertlist[i].curvature = 1.0/2.0/A_mixed 
      }
   }

















}


