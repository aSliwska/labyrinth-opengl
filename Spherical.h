#pragma once

#include "pch.h"

struct Spherical
{
public:
    float distance, theta, phi;

    Spherical(float gdistance, float gtheta, float gphi);
    float getX();
    float getY();
    float getZ();
};