#pragma once
namespace oglut{
    class Color
    {
    public:
        double r;
        double g;
        double b;
        
        Color(unsigned char r, unsigned char g, unsigned  char b);
        Color(double r, double g, double b);
        void setRGB(double r, double g, double b);
        void setRGB(unsigned char r, unsigned char g, unsigned  char b);
        ~Color(void);
    };
}
