#include "pch.h"
#include "Spherical.h"

Spherical::Spherical() : Spherical(0, 0, 0) {}

Spherical::Spherical(float distance, float theta, float phi) : distance(distance), theta(theta), phi(phi) { }

float Spherical::getDistance() { return distance; }
float Spherical::getTheta() { return theta; }
float Spherical::getPhi() { return phi; }

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

UnlockedSpherical::UnlockedSpherical(float distance, float theta, float phi) : Spherical(distance, theta, phi) {}

void UnlockedSpherical::setTheta(float theta) 
{
	this->theta = theta;
}

LockedSpherical::LockedSpherical(float distance, float theta, float phi) : Spherical(distance, theta, phi) 
{ 
	x = distance * cos(phi) * cos(theta);
	y = distance * sin(phi);
	z = distance * cos(phi) * sin(theta);
}

float LockedSpherical::getX() { return x; }
float LockedSpherical::getY() { return y; }
float LockedSpherical::getZ() { return z; }
