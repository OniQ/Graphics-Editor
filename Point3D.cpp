#include "stdafx.h"
#include "Point3D.h"

using namespace oglut;

Point3D::Point3D()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Point3D* Point3D::lineCenter(Point3D p2){
    return new Point3D((x + p2.x) / 2, (y + p2.y) / 2, (z + p2.z) / 2);
};

Point3D::Point3D(GLfloat x, GLfloat y, GLfloat z)
{
    this->x = x;
    this->y = y;
    this->z = z;
} 

Point3D::~Point3D()
{
}
