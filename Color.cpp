#include "stdafx.h"
#include "Color.h"

using namespace oglut;

Color::Color(unsigned char r, unsigned char g, unsigned char b){
    this->r = r / 255.0;
    this->g = g / 255.0;
    this->b = b / 255.0;
} 

Color::Color(double r, double g, double b){
    this->r = r;
    this->g = g;
    this->b = b;
}

void Color::setRGB(unsigned char r, unsigned char g, unsigned  char b){
    this->r = r / 255.0;
    this->g = g / 255.0;
    this->b = b / 255.0;
}

void Color::setRGB(double r, double g, double b){
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::~Color(void)
{
}
