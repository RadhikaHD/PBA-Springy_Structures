

#include "Particle.h"

Particle::Particle()
{
    
}

Particle::Particle( const Vector3d &p, const Vector3d &v, const Vector3d &a,  const double mass, const Vector3d &f)
{
    setParticlev(v);
    setParticlep(p);
    setParticlea(a);
    setParticlemass(mass);
    setParticlef(f);
}

Particle::~Particle()
{
    
}

void Particle::setParticlev(const Vector3d &v)
{
    Velocity = v;
}

void Particle::setParticlep(const Vector3d &p)
{
    Position = p;
}

void Particle::setParticlea(const Vector3d &a)
{
    Acceleration = a;
}

void Particle::setParticlemass(const double mass)
{
    Particle::mass  = mass;
}


void Particle::setParticlef(const Vector3d &f)
{
    Force = f;
}






