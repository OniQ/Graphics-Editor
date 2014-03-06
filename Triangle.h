#pragma once
#include "Shape.h"
#include "Color.h"
#include "Point3D.h"
#include <cstdlib>
namespace oglut{
    class Triangle :
        public Shape
    { 
    public:
        bool insider;
        GLuint tex;
        Point3D* p1;
        Point3D* p2;
        Point3D* p3; 
        Point3D* cp1p2;
        Point3D* cp2p3;
        Point3D* cp3p1; 

        Triangle(Point3D* p1, Point3D* p2, Point3D* p3); 
        void draw();
        void setTexture(GLuint _tex);
        void rndp(Point3D* p, int div); 
        void findSideCenter(int div); 
        ~Triangle();
    };
}

