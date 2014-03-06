#ifndef SHAPE_H
#define SHAPE_H
#include "Color.h"
namespace oglut{
    class Shape
    {
    public:
        Color* color;
        bool hidden;
        GLfloat t_x;
        GLfloat t_y;
        GLfloat t_z;
        GLfloat rotation;
        GLfloat ambient[3];
        GLfloat diffuse[3];
        GLfloat specular[3];
        GLfloat shininess;
        GLfloat refraction;
        GLfloat transparency;
        UINT primitive;
        virtual void draw() = 0;
    protected:
        Shape(){};
    };
}
#endif
