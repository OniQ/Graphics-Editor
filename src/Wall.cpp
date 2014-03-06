#include "stdafx.h"
#include "Wall.h"

using namespace oglut;

Wall::Wall(Point3D p1, Point3D p2, Point3D p3, Point3D p4)
{
    corners.push_back(p1);
    corners.push_back(p2);
    corners.push_back(p3);
    corners.push_back(p4);
}

Wall::Wall(Point3D p1, Point3D p2, Point3D p3)
{
    corners.push_back(p1);
    corners.push_back(p2);
    corners.push_back(p3);
}

Wall::Wall(std::vector<Point3D> points){
    corners = points;
}

Wall::Wall(std::vector<Point3D> points, std::vector<TextureVertex2D> tv){
    corners = points;
    texVertex = tv;
}

void Wall::draw(){
    glBegin(primitive); 
    unsigned int size = corners.size();
    for (unsigned j = 0; j < size; j++){
        if (!paint.empty())
            glColor3d(paint[j].r, paint[j].g, paint[j].b);
        if (!texVertex.empty())
            glTexCoord2f(texVertex[j].x, texVertex[j].y);
        glVertex3d(corners[j].x, corners[j].y, corners[j].z);
    } 
    glEnd();
}

Wall::~Wall()
{
}
