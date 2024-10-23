#pragma once

#include "pch.h"

class Spherical {
protected:
    float distance, theta, phi;

public:
    Spherical();
    Spherical(float distance, float theta, float phi);

    float getDistance();
    float getTheta();
    float getPhi();
    virtual float getX();
    virtual float getY();
    virtual float getZ();
};

class UnlockedSpherical : public Spherical {
public:
    UnlockedSpherical(float distance, float theta, float phi);

    void setTheta(float theta);
};

class LockedSpherical : public Spherical {
protected:
    float x, y, z;

public:
    LockedSpherical(float distance, float theta, float phi);

    float getX() override;
    float getY() override;
    float getZ() override;
};