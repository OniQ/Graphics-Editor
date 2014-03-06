#pragma once

namespace oglut{
    class Point3D
    {
    public:
        GLfloat x;
        GLfloat y;
        GLfloat z;
        Point3D();
        Point3D(GLfloat x, GLfloat y, GLfloat z);
        Point3D* lineCenter(Point3D p2);
        ~Point3D();
    };
}
