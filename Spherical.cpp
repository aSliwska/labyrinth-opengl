#include "pch.h"
#include "Spherical.h"

Spherical::Spherical(float gdistance, float gtheta, float gphi) : distance(gdistance), theta(gtheta), phi(gphi) { }

float Spherical::getX() 
{ 
	return distance * cos(phi) * cos(theta); 
}

float Spherical::getY() 
{ 
	return distance * sin(phi); 
}

float Spherical::getZ() 
{ 
	return distance * cos(phi) * sin(theta); 
}
