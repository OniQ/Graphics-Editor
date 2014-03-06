#define CAMERASPEED 0.03f 

//////////////////////////////////////
//The tVector3 Struct
//////////////////////////////////////
struct tVector3    // Extended 3D Vector Struct
{
    tVector3() {} // Struct Constructor
    tVector3(float new_x, float new_y, float new_z) // Init Constructor     
    {
        x = new_x; y = new_y; z = new_z;
    }
    // overload + operator
    tVector3 operator+(tVector3 vVector) { return tVector3(vVector.x + x, vVector.y + y, vVector.z + z); }
    // overload - operator
    tVector3 operator-(tVector3 vVector) { return tVector3(x - vVector.x, y - vVector.y, z - vVector.z); }
    // overload * operator
    tVector3 operator*(float number)     { return tVector3(x*number, y*number, z*number); }
    // overload / operator
    tVector3 operator/(float number)     { return tVector3(x / number, y / number, z / number); }

    float x, y, z;                        // 3D vector coordinates
};


//////////////////////////////////////
//The CCamera Class
//////////////////////////////////////
class CCamera
{
public:

    tVector3 mPos;
    tVector3 mView;
    tVector3 mUp;
    int centerX;
    int centerY;
    //NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

    void Strafe_Camera(float speed);

    //NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

    void Mouse_Move(int wndWidth, int wndHeight);
    void Move_Camera(float speed);
    void Rotate_View(float speed);
    void Position_Camera(float pos_x, float pos_y, float pos_z,
    float view_x, float view_y, float view_z,
    float up_x, float up_y, float up_z);
};
// TODO: reference additional headers your program requires here
