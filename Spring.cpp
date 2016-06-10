
#include "Spring.h"

Spring::Spring()
{
    
}

Spring::Spring(const double springConstant, const double dampingConstant, const double restLength, const int pi, const int pj)
{
    setSpringk(springConstant);
    setSpringd(dampingConstant);
    setSpringl(restLength);
    setSpringpi(pi);
    setSpringpj(pj);
}

Spring::~Spring()
{
    
}

void Spring::setSpringk(const double k)
{
    springConstant = k;
}

void Spring::setSpringd(const double d)
{
    dampingConstant = d;
}

void Spring::setSpringl(const double l)
{
    restLength = l;
}

void Spring::setSpringpi(const int i)
{
    pi  = i;
}


void Spring::setSpringpj(const int j)
{
    pj = j;
}

