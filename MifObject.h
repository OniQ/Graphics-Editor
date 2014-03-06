#pragma once
#include "Wall.h"
#include "Color.h"
#include "Point3D.h"
#include "TextureVertex2D.h"
#include "Texture.h" 
#include <fstream>
#include <sstream>
#include <vector>
#include "Shape.h"

namespace oglut{
    class MifObject : public Shape
    {
        std::vector<Point3D> points;
        std::vector<TextureVertex2D> texVertex;
        std::vector<Color> colors;
        std::vector<Wall> figure;
        std::string textureName;
        std::stringstream& removeComments(std::string file);
        UINT TextureArray[1]; 

        void load(std::string file);
        void loadMtl(std::string mtlfile, std::string materialName);
    public:
        MifObject(std::string fName);
        void draw();
        ~MifObject();
    };
}
