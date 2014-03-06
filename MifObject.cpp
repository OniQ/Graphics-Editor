#include "stdafx.h"
#include "MifObject.h"

using namespace oglut;
using namespace std;

MifObject::MifObject(string fName)
{
    load(fName);
    hidden = false;
    t_x = 0.0f;
    t_y = 0.0f;
    t_z = 0.0f;
    rotation = 0.0f;
}

stringstream& MifObject::removeComments(string file){
    ifstream myfile(file.c_str());
    stringstream* fileStream = new stringstream;
    string line;
    unsigned pos;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            pos = line.find_first_of('#');
            if (pos == std::string::npos){
                *fileStream << line << endl;
            }
            else{
                line = line.substr(0, pos);
                if (line.length()){
                    *fileStream << line << endl;
                }
            }
        }
        myfile.close();
    }
    else{
        cerr << "Unable to open file";
    }
    return *fileStream;
}

void MifObject::loadMtl(string mtlfile, string materialName){
    stringstream& mtlfileStream = removeComments(mtlfile);
    string mName;
    string type;
    int index = 0;
    bool rightMeterial = false;
    do{
        mtlfileStream >> type;
        if (type.compare("newmtl") == 0){
            mtlfileStream >> mName;
            if (mName.compare(materialName) == 0)
                rightMeterial = true;
        }
        else if (rightMeterial){
            if (type.compare("map_Kd") == 0){
                mtlfileStream >> textureName;
                BMP_Texture(TextureArray, textureName.c_str(), index);
                index++; 
            }
            else if (type.compare("Ns") == 0){
                string line;
                getline(mtlfileStream, line);
                std::istringstream linestream(line);
                linestream >> shininess;
            }
            else if (type.compare("Ni") == 0){
                string line;
                getline(mtlfileStream, line);
                std::istringstream linestream(line);
                linestream >> refraction;
            }
            else if (type.compare("Ka") == 0){
                string line;
                getline(mtlfileStream, line);
                std::istringstream linestream(line);
                linestream >> ambient[0] >> ambient[1] >> ambient[2];
            }
            else if (type.compare("Kd") == 0){
                string line;
                getline(mtlfileStream, line);
                std::istringstream linestream(line);
                linestream >> diffuse[0] >> diffuse[1] >> diffuse[2];
            }
            else if (type.compare("Ks") == 0){
                string line;
                getline(mtlfileStream, line);
                std::istringstream linestream(line);
                linestream >> specular[0] >> specular[1] >> specular[2];
            }
        } 
        type.clear();
    } while (mtlfileStream);
}

void MifObject::load(string file){
    stringstream& fileStream = removeComments(file);
    string mtlFileName;
    string type;
    string s;
    GLfloat x;
    GLfloat y;
    GLfloat z;
    double r;
    double g;
    double b;
    int wall[4];
    do{
        fileStream >> type;
        if (type.compare("v6") == 0){
            fileStream >> x >> y >> z;
            points.push_back(Point3D(x, y, z));
            fileStream >> r >> g >> b;
            colors.push_back(Color(r, g, b));
        }
        else if (type.compare("v") == 0){
            fileStream >> x >> y >> z;
            points.push_back(Point3D(x, y, z));
        }
        else if (type.compare("vt") == 0){
            fileStream >> x >> y;
            texVertex.push_back(TextureVertex2D(x, y));
        }
        else if (type.compare("s4") == 0){
            for (int i = 0; i < 4; i++){
                fileStream >> wall[i];
                wall[i] -= 1;
            } 
            Wall w(points[wall[0]], points[wall[1]], points[wall[2]], points[wall[3]]);
            w.paint.push_back(colors.at(wall[0]));
            w.paint.push_back(colors.at(wall[1]));
            w.paint.push_back(colors.at(wall[2]));
            w.paint.push_back(colors.at(wall[3]));
            figure.push_back(w);
            getline(fileStream, s);
        }
        else if (type.compare("s3") == 0){
            for (int i = 0; i < 3; i++){
                fileStream >> wall[i];
                wall[i] -= 1;
            }
            Wall w(points[wall[0]], points[wall[1]], points[wall[2]]);
            w.paint.push_back(colors.at(wall[0]));
            w.paint.push_back(colors.at(wall[1]));
            w.paint.push_back(colors.at(wall[2]));
            figure.push_back(w);
            getline(fileStream, s);
        }
        else if (type.compare("f") == 0){
            int s = 0;
            int tv = 0;
            string line;
            string word;
            unsigned pos;
            getline(fileStream, line);
            pos = line.find_first_of('/');
            std::vector<Point3D> vertex;
            std::vector<TextureVertex2D> textureVertex;
            std::istringstream linestream(line);
            if (linestream){
                if (pos == std::string::npos){
                    linestream >> s;
                    do{
                        vertex.push_back(points[s - 1]);
                        linestream >> s;
                    } while (linestream);
                    Wall w(vertex);
                    figure.push_back(w);
                }
                else{
                    linestream >> word;
                    sscanf(word.c_str(), "%d/%d", &s, &tv);
                    do{
                        vertex.push_back(points[s - 1]);
                        textureVertex.push_back(texVertex[tv - 1]);
                        linestream >> word;
                        sscanf(word.c_str(), "%d/%d", &s, &tv);
                    } while (linestream);
                    Wall w(vertex, textureVertex);
                    figure.push_back(w);
                } 
            }
            else{
                getline(fileStream, line);
            }
        }
        else if (type.compare("mtllib") == 0){ 
            fileStream >> mtlFileName; 
        }
        else if (type.compare("usemtl") == 0){
            string materialName;
            fileStream >> materialName;
            loadMtl(mtlFileName, materialName);
        }
        else if (type.compare("cf") == 0){
            glEnable(GL_CULL_FACE);
        }
        else if (type.compare("sf") == 0){
            glShadeModel(GL_FLAT);
        }
        else if (type.compare("ss") == 0){
            glShadeModel(GL_SMOOTH);
        } 
        type.clear();
    } while (fileStream);
}

void MifObject::draw(){
    glEnable(GL_POLYGON_SMOOTH);
    glColor3d(1, 0.9, 0.8);
    glBindTexture(GL_TEXTURE_2D, TextureArray[0]);
    for(vector<Wall>::iterator it = figure.begin(); it != figure.end(); ++it) {
       it->primitive = this->primitive;
       it->draw();     
    } 
}

MifObject::~MifObject()
{
}
