#pragma once
#include "Point3D.h"
#include "TextureVertex2D.h"
#include "Shape.h"
#include <vector>

namespace oglut{

    class Wall : public Shape
    {
    public:
        std::vector<Point3D> corners;
        std::vector<TextureVertex2D> texVertex;
        std::vector<Color> paint;

        Wall(){};
        Wall(Point3D p1, Point3D p2, Point3D p3, Point3D p4);
        Wall(Point3D p1, Point3D p2, Point3D p3);
        Wall(std::vector<Point3D> points);
        Wall(std::vector<Point3D> points, std::vector<TextureVertex2D> tv);
        void draw();
        ~Wall();
    };

}
