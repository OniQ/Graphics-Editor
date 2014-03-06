#include "stdafx.h"
#include "Triangle.h"

using namespace oglut;

Triangle::Triangle(Point3D* p1, Point3D* p2, Point3D* p3)
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    insider = false;
}

void Triangle::findSideCenter(int div){
    cp1p2 = p1->lineCenter(*p2);
    cp2p3 = p2->lineCenter(*p3);
    cp3p1 = p3->lineCenter(*p1);
    rndp(cp1p2, div);
    rndp(cp2p3, div);
    rndp(cp3p1, div);
}

void Triangle::rndp(Point3D* p, int div){
    float r = (float)rand();
    float offset =  sin(r)*0.5 / div;
    //p.x *= offset;
    p->y += offset;
    //p.z += offset;
}

void Triangle::setTexture(GLuint _tex){
    tex = _tex;
}

void Triangle::draw(){ 
    glBegin(GL_POLYGON); 
        glTexCoord2f(1.0, 0.0);  glVertex3f(p1->x, p1->y, p1->z); 
        glTexCoord2f(0.5, 1.0);  glVertex3f(p2->x, p2->y, p2->z); 
        glTexCoord2f(0.0, 0.0);  glVertex3f(p3->x, p3->y, p3->z);
    glEnd(); 
}

Triangle::~Triangle()
{
}
