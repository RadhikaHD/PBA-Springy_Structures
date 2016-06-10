
#ifndef Spring_h
#define Spring_h

#include <stdio.h>

#include "Vector.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

//#include "GL/gl.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif



class Spring
{
public:
    
    double springConstant;
    double dampingConstant;
    double restLength;
    int pi;
    int pj;
    
    
    Spring();
    Spring(const double springConstant, const double dampingConstant, const double restLength, const int pi, const int pj);
    ~Spring();
    
    void setSpringk(const double springConstant);
    void setSpringd(const double dampingConstant);
    void setSpringl(const double restLength);
    void setSpringpi(const int pi);
    void setSpringpj(const int pj);
        
};

#endif
