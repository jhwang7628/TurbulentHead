#ifndef VECTOR3_H
#define VECTOR3_H

#ifndef CMATH
#define CMATH
#include <cmath>
#include <vector>
#endif

template <class T> 
class Vector3 : public std::vector<T>
{ 
    public: 
        T x, y, z; 
        // -------------- Constructors ------------------ //
        Vector3<T>() { x = y = z = 0; }
        Vector3<T>(T xVal, T yVal, T zVal)
        {
            x = xVal;
            y = yVal; 
            z = zVal; 
        }


        // -------------- Basic Arithmetics ------------------ //

        // in-place addition
        Vector3<T>& add(T targetX, T targetY, T targetZ)
        {
            x += targetX; 
            y += targetY; 
            z += targetZ; 
            return *this;
        }

        // in-place addition
        Vector3<T>& add(Vector3<T> target)
        { 
            return add(target.x, target.y, target.z);
        } 

        // in-place subtraction
        Vector3<T>& sub(T targetX, T targetY, T targetZ)
        {
            x -= targetX; 
            y -= targetY; 
            z -= targetZ; 
            return *this;
        }

        // in-place subtraction
        Vector3<T>& sub(Vector3<T> target)
        { 
            return sub(target.x, target.y, target.z);
        } 

        // in-place multiplication 
        Vector3<T>& mul(T fac)
        {
            x *= fac; 
            y *= fac; 
            z *= fac; 
            return *this;
        }

        // in-place multiplication 
        Vector3<T>& mul(T targetX, T targetY, T targetZ)
        {
            x *= targetX; 
            y *= targetY; 
            z *= targetZ; 
            return *this;
        }

        // in-place multiplication 
        Vector3<T>& mul(Vector3<T> target)
        { 
            return mul(target.x, target.y, target.z);
        } 

        // in-place division 
        Vector3<T>& div(T fac)
        {
            x /= fac; 
            y /= fac; 
            z /= fac; 
            return *this;
        }

        // in-place division 
        Vector3<T>& div(T targetX, T targetY, T targetZ)
        {
            x /= targetX; 
            y /= targetY; 
            z /= targetZ; 
            return *this;
        }

        // in-place division 
        Vector3<T>& div(Vector3<T> target)
        { 
            return div(target.x, target.y, target.z);
        } 


        // -------------- Vector Arithmetics ------------------ //
       
        // in-place cross product
        Vector3<T> cross(Vector3<T> v) 
        {
           T a1=x,a2=y,a3=z,v1=v.x,v2=v.y,v3=v.z; 

           x = a2*v3 - a3*v2; 
           y = a3*v1 - a1*v3; 
           z = a1*v2 - a2*v1; 

           return *this;
        }

        // vector dot product
        T dot(Vector3<T> v)
        { 
            return x*v.x + y*v.y + z*v.z;
        }

        // norm squared calculation
        T normSq() 
        {
           return pow(x,(T)2.0) + pow(y,(T)2.0) + pow(z,(T)2.0);
        }

        // norm calculation
        T norm() 
        {
           return sqrt(this->normSq());
        }

        // in-place vector normalization
        Vector3<T>& normalize()
        { 
           T normFactor = this->norm();
           this->div(normFactor); 
           return *this;
        }

        
        // -------------- Additional Helpers ------------------ //

        void zero()
        {
            x = y = z = 0; 
        } 




        // -------------- Operator Overload ------------------ //
        
        friend std::ostream &operator<<(std::ostream& os, const Vector3<T>& V)
        {
            os << "{" << V.x << ", " << V.y << ", " << V.z << "}"; 
            return os;
        }

};

#endif
