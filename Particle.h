
#ifndef Particle_h
#define Particle_h

#include "Vector.h"
#include "Spring.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

//#include "GL/gl.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif



class Particle
{
public:
    
     Vector3d Position;
     Vector3d Velocity;
    
     Vector3d Acceleration;
     double mass;
     Vector3d Force;
    
    
    Particle();
    Particle( const Vector3d &p, const Vector3d &v, const Vector3d &a, const double mass, const Vector3d &f );
    ~Particle();

    
    void setParticlev(const Vector3d &v);
    void setParticlep(const Vector3d &p);
    void setParticlea(const Vector3d &a);
    void setParticlemass(const double mass);
    void setParticlef(const Vector3d &f);

    
};

#endif